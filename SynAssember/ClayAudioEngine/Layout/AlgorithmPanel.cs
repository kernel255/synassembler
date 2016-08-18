using EUShelves;
using SynthPanels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;

namespace ClayAudioEngine.Layout
{
	public class AlgorithmPanel
	{
		EUShelf m_LocalShelf;

		public AlgorithmPanel(Canvas canvas, EUShelf storeShelf)
		{
			m_CanvasPanel = canvas;
			m_CurrentConnection = new EUConnection(new Point(0, 0), new Point(0, 0), canvas);
			m_CurrentConnection.place();
			m_StoreShelf = storeShelf;

			m_LocalShelf = new EUShelf(6, 1000, 200, 0, 0, 4);
			m_LocalShelf.addToPanel(m_CanvasPanel);
			m_LocalShelf.DrawLedgePlaceHolders();
		}

		private AlgorithmGraph m_currentAlgorithmGraph;
		public void setCurrentAlgorithmGraph(AlgorithmGraph graph)
		{
			m_currentAlgorithmGraph = graph;
		}

		public void PreviewMouseMove(object sender, MouseEventArgs e)
		{
			if (m_IsConnecting)
			{
				Point endPoint = e.GetPosition(m_CanvasPanel);
				m_CurrentConnection.EndPoint = endPoint;
				moveableInOut.getGlyph(m_CurrentConnection.PortType).MoveTo(endPoint);

			}

		}

		public void activateConnection(bool enable)
		{
			m_CurrentConnection.setTransparent(!enable);
		}

		public void MouseDown(object sender, MouseButtonEventArgs e)
		{
			if (e.LeftButton == MouseButtonState.Pressed && !m_IsConnecting)
			{
				m_IsConnecting = true;
				if (e.Source is Shape)
				{
					if (e.Source is TextBlock)
					{
						e.Handled = false;
						return;
					}
					string name = (e.Source as Shape).Name;
					int euIndex = InputOutputGlyph.getEUAudioEngineIdByCombined(name);
					ElaborationUnitGlyphInstance euSrc = m_currentAlgorithmGraph.getEUInstanceByAudioEngineId(euIndex);
					int portIndex = InputOutputGlyph.getPortIndexByCombined(name);
					// Only if the port is an output can connect
					if (InputOutputGlyph.IsOutputByCombibed(name))
					{
						m_CurrentConnection.setSourceEU(euSrc, portIndex);
						m_SourceEUStringDescr = name;
						m_ConnectionStartPoint = e.GetPosition(m_CanvasPanel);
						PointCollection coll = new PointCollection();
						coll.Add(m_ConnectionStartPoint);
						activateConnection(true);
						m_CurrentConnection.StartPoint = m_ConnectionStartPoint;
						moveableInOut.setCanvas(m_CanvasPanel);
						Port.PortType type = InputOutputGlyph.getTypeByCombined(name);
						moveableInOut.setPortType(type);
						moveableInOut.getGlyph(type).MoveTo(m_ConnectionStartPoint);
					}
				}
			}

		}

		public void Drop(object sender, DragEventArgs e, Int32 hWnd)
		{
			if (e.Data.GetDataPresent(System.Windows.DataFormats.Text))
			{
				object draggedData = e.Data.GetData(System.Windows.DataFormats.Text);
				if (draggedData is String)
				{
					String draggedEUCombinedStr = (String)draggedData;
					String factoryIndexStr = ElaborationUnitGlyph.getFactoryIndexByCombined(draggedEUCombinedStr);
					String euIndexStr = ElaborationUnitGlyph.getEUIndexByCombined(draggedEUCombinedStr);
					int factoryIndex = Convert.ToInt32(factoryIndexStr);
					int euIndex = Convert.ToInt32(euIndexStr);
					Point p = e.GetPosition(m_CanvasPanel);
					int physicalInstanceId = -1;

					ElaborationUnitFactory fact = AudioEngineWrapper.getDefault().getFactoryById(factoryIndex);
					ElaborationUnitDescription euDescr = AudioEngineWrapper.getEUDescriptionById(fact, euIndex);
					if (euDescr.Physical)
					{
						if (euDescr.InstancesCount == 0)
						{
							string msg = String.Format("The Elaboration Unit: {0} is not available", euDescr.Name);
							throw new Exception(msg);
						}
						else
						{
							String physInstId = ElaborationUnitGlyph.getPhysicalInstaceByCombined(draggedEUCombinedStr);
							physicalInstanceId = Int32.Parse(physInstId);
							m_StoreShelf.removePhysicalEUInstance(factoryIndex, euIndex, physicalInstanceId);
						}
					}

					try
					{
						AudioEngineWrapper.getDefault()._setHwnd((int)hWnd);
						Rectangle rect = m_LocalShelf.GetNearestRect(p);
						if (rect != null)
						{
							double x = Canvas.GetLeft(rect);
							double y = Canvas.GetTop(rect);
							ElaborationUnitGlyphInstance euInst = m_currentAlgorithmGraph.addElaborationUnitGlyph(x, y, euDescr, fact, physicalInstanceId);
						}


					}
					catch (Exception ex)
					{
						MessageBox.Show(ex.Message);
						return;
					}

					//ISynthPanel synPanel = AudioEngineWrapper.createNewPanel(fact.getName(), euDescr.Name, euIndex);
					//SynthPanelManager.getDefault().AddSynthPanel(synPanel);
					//SynthPanelArea.Children.Add(synPanel);
				}
			}

		}

		private void resetIOSearch()
		{
			ioConnectionFound = false;
			m_CurrentIOShape = null;
		}

		public static HitTestResultBehavior testResultCallback(HitTestResult res)
		{
			if (res.VisualHit is Shape)
			{
				Shape shape = res.VisualHit as Shape;
				String name = shape.Name;
				if (InputOutputGlyph.IsShapeNameIO(name))
				{
					String pippo = name + "C";
					ioConnectionFound = true;
					m_CurrentIOShape = res.VisualHit as Shape;
					return HitTestResultBehavior.Stop;
				}
			}
			ioConnectionFound = false;
			return HitTestResultBehavior.Continue;
		}


		public void MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
		{
			if (m_SourceEUStringDescr != null)
			{
				if (m_IsConnecting)
				{
					Point p = e.GetPosition((UIElement)sender);
					resetIOSearch();
					VisualTreeHelper.HitTest(m_CanvasPanel, null, new HitTestResultCallback(testResultCallback),
						new PointHitTestParameters(p));
					if (ioConnectionFound)
					{
						String dstName = m_CurrentIOShape.Name;
						m_CurrentConnection.EndPoint = p;
						if (InputOutputGlyph.areIOCompatible(m_SourceEUStringDescr, dstName))
						{
							EUConnection conn = new EUConnection(m_CurrentConnection);
							int euIndex = InputOutputGlyph.getEUAudioEngineIdByCombined(dstName);
							ElaborationUnitGlyphInstance euDst = m_currentAlgorithmGraph.getEUInstanceByAudioEngineId(euIndex);
							int portIndex = InputOutputGlyph.getPortIndexByCombined(dstName);
							euDst.setInputConnected(portIndex);
							conn.setDestinationEU(euDst, portIndex);
							conn.EndPoint = euDst.GetInputCenter(portIndex);
							m_currentAlgorithmGraph.addConnection(conn);
							resetIOSearch();
							
						}
					}
					moveableInOut.Hide();
				}
			}
			m_SourceEUStringDescr = null;
			m_IsConnecting = false;
			activateConnection(false);
		}

		public void SizeChanged(object sender, SizeChangedEventArgs e)
		{
			try {
				m_CanvasPanel.Width = SynthPanelManager.getDefault().GetWidth();
			} catch {
			}
			try {
				m_CanvasPanel.Height = SynthPanelManager.getDefault().GetHeight();
			} catch {
			}
		}

		public void CleanUp()
		{
			m_CanvasPanel.Children.Clear();
		}

		Canvas m_CanvasPanel;
		private bool m_IsConnecting = false;
		private EUConnection m_CurrentConnection;
		MoveableInputOutputGliph moveableInOut = new MoveableInputOutputGliph();
		private String m_SourceEUStringDescr = null;
		private Point m_ConnectionStartPoint;
		private static bool ioConnectionFound = false;
		private static Shape m_CurrentIOShape;
		private EUShelf m_StoreShelf;

		const double C_DefaultWidth = 800;
		const double C_DefaultHeight = 500;

	}
}
