using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Runtime.InteropServices;
using EUShelves;
using Microsoft.Win32;
using System.Xml;
using System.Windows.Interop;
using System.IO;
using System.Diagnostics;
using ClayAudioEngine;
using SynthPanels;
using GeneralUtils;

namespace SynAssember
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport("kernel32.dll", ExactSpelling = true)]
        private static extern IntPtr GetConsoleWindow();


        //private bool m_SynthNew = false;
        private bool m_SynthSaved = true;

        private bool hwndRetrieved = false;

		private void fileManagementBindings() {
			CommandBinding saveBind = new CommandBinding(ApplicationCommands.Save);
			saveBind.CanExecute += canSaveExecute;
			saveBind.Executed += saveExecuted;
			CommandBindings.Add(saveBind);
			CommandBinding openBind = new CommandBinding(ApplicationCommands.Open);
			openBind.CanExecute += canOpenExecute;
			openBind.Executed += openExecuted;
			CommandBindings.Add(openBind);
			CommandBinding newBind = new CommandBinding(ApplicationCommands.New);
			newBind.CanExecute += canNewExecute;
			newBind.Executed += newExecuted;
			CommandBindings.Add(newBind);
			CommandBinding saveAsBind = new CommandBinding(ApplicationCommands.SaveAs);
			saveAsBind.CanExecute += canSaveAsExecute;
			saveAsBind.Executed += saveAsExecute;
			CommandBindings.Add(saveAsBind);
		}

		EUShelf m_Shelf;

		Options options;

		Facilities facilities;

		public MainWindow()
		{
			InitializeComponent();
			fileManagementBindings();
			facilities = new Facilities();
			String currentFolder = Directory.GetCurrentDirectory();
			Trace.Write("Current folder=" + currentFolder);

			if (!initAudioEngine(currentFolder))
			{
				MessageBox.Show("Error while initializing AudioEngine");
				//TODO: Show some message
				//Application.Current.Exit();
			}
			else
			{
				currentFolder = Directory.GetCurrentDirectory();
				Trace.Write("Current folder=" + currentFolder);
				m_Shelf = new EUShelf(10, 800, 200, 0, 0, AudioEngineWrapper.getDefault().getFactories());
				m_Shelf.addToPanel(LeftPanel);
				CurrentAlgorithm = new AlgorithmGraph(RightPanel);
				//facilities.setCurrentChangeAlgorithm(m_CurrentAlgorithGraph);
				m_CurrentConnection = new EUConnection(new Point(0, 0), new Point(0, 0), RightPanel);
				m_CurrentConnection.place();
			}

			SynthPanelManager.getDefault().SetCanvas(SynthPanelArea);

			options = new Options();

			RetrieveSettings();
		}

        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown(0);
        }

        private void canSaveExecute(object sender, CanExecuteRoutedEventArgs e)
        {
			e.CanExecute = CurrentAlgorithm.hasChanged();
        }

        private void saveExecuted(object sender, ExecutedRoutedEventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Title = "Save a synthesizer";
            dlg.ShowDialog(this);
            String filename = dlg.FileName;
            if (filename.Length != 0)
            {
				CurrentAlgorithm.write(filename);
            }
            MessageBox.Show("File saved successfully.");
        }

		private void canSaveAsExecute(object sender, CanExecuteRoutedEventArgs e)
		{
			e.CanExecute = true;
		}

		private void saveAsExecute(object sender, ExecutedRoutedEventArgs e)
		{
			SaveFileDialog dlg = new SaveFileDialog();
			dlg.Title = "Save a synthesizer";
			dlg.ShowDialog(this);
			String filename = dlg.FileName;
			if (filename.Length != 0)
			{
				CurrentAlgorithm.write(filename);
			}
			MessageBox.Show("File saved successfully.");
		}

        private void canOpenExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void openExecuted(object sender, ExecutedRoutedEventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            dlg.CheckFileExists = true;
            dlg.Multiselect = false;
            dlg.Title = "Open a Synthesizer";
            dlg.ShowDialog(this);
            if(dlg.FileName.Length!=0)
            {
                XmlTextReader reader = new XmlTextReader(dlg.FileName);
				CurrentAlgorithm = new AlgorithmGraph(RightPanel);
				//facilities.setCurrentChangeAlgorithm(m_CurrentAlgorithGraph);
                Int32 hWnd = (Int32)getWindowHwnd();
				AudioEngineWrapper.getDefault()._setHwnd((int)hWnd);
                try {
					CurrentAlgorithm.read(reader);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error while reading " + dlg.FileName);
                    return;
                }

                Properties.Settings.Default.LoadAtStartUpPath = dlg.FileName;
            }
        }

		private void LoadLastSynth()
		{
			if (Properties.Settings.Default.LoadAtStartUp == true)
			{
				string filename = Properties.Settings.Default.LoadAtStartUpPath;
				if (filename != null && filename.Length > 0)
				{
					XmlTextReader reader = new XmlTextReader(filename);
					CurrentAlgorithm = new AlgorithmGraph(RightPanel);
					//facilities.setCurrentChangeAlgorithm(m_CurrentAlgorithGraph)
					Int32 hWnd = (Int32)getWindowHwnd();
					AudioEngineWrapper.getDefault()._setHwnd((int)hWnd);
					try
					{
						CurrentAlgorithm.read(reader);
					}
					catch (Exception ex)
					{
						string msg = "Error while reading: " + filename;
						MessageBox.Show(msg);
					}
				}
			}

		}

        private void canNewExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void newExecuted(object sender, ExecutedRoutedEventArgs e)
        {
            if (!m_SynthSaved)
            {
                if (MessageBox.Show(this, "The current Synthesizer has been modifed. Do you want to save it?", "SynAssembler", MessageBoxButton.YesNo, MessageBoxImage.Question, MessageBoxResult.Yes) == MessageBoxResult.Yes)
                {
                }
            }
        }

        private bool initAudioEngine(string applicationFolder)
        {
            //StringBuilder dllPath = new StringBuilder("D:\\Mine\\Audinos\\SynAssembler-Apr2010\\SynAssemblerMix\\AudioEngine\\Debug\\");
            StringBuilder dllPath = new StringBuilder(applicationFolder);
			ClayAudioEngine.AudioEngineWrapper.getDefault().init(dllPath, 0, 48000, 16, 1);

            return true;
        }

        private Point m_DragStartPoint;
        private bool m_IsDragging = false;


        private void LeftPanel_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            m_DragStartPoint = e.GetPosition(null);
        }

        private void LeftPanel_PreviewMouseMove(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed && !m_IsDragging)
            {
                Point position = e.GetPosition(null);
                if (Math.Abs(position.X - m_DragStartPoint.X) > SystemParameters.MinimumHorizontalDragDistance ||
                                    Math.Abs(position.Y - m_DragStartPoint.Y) > SystemParameters.MinimumVerticalDragDistance)
                {
                    StartDrag(e);
                }
            }
        }

        private void StartDrag(MouseEventArgs e)
        {
            m_IsDragging = true;
            if (e.Source is Rectangle)
            {
                Rectangle rect = (Rectangle) e.Source;
                String name = rect.Name;
                DataObject data = new DataObject(System.Windows.DataFormats.Text.ToString(), name);
                DragDropEffects de = DragDrop.DoDragDrop(this.LeftPanel, data, DragDropEffects.Move);
            }
            m_IsDragging = false;
        }


        private IntPtr windowHwnd;
        private IntPtr getWindowHwnd()
        {
            if (!hwndRetrieved)
            {
                WindowInteropHelper localWindowHwnd = new WindowInteropHelper(this);
                windowHwnd = localWindowHwnd.Handle;
                hwndRetrieved = true;
            }
            return windowHwnd;
        }

        private void RightPanel_Drop(object sender, DragEventArgs e)
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
                    Point p = e.GetPosition(RightPanel);
                    int physicalInstanceId = -1;

					ElaborationUnitFactory fact = AudioEngineWrapper.getDefault().getFactoryById(factoryIndex);
                    ElaborationUnitDescription euDescr = AudioEngineWrapper.getEUDescriptionById(fact, euIndex);
                    if (euDescr.Physical)
                    {
                        if (euDescr.InstancesCount == 0)
                        {
                            string msg = String.Format("The Elaboration Unit: {0} is not available", euDescr.Name);
                            MessageBox.Show(this, msg);
                            return;
                        }
                        else
						{
							String physInstId = ElaborationUnitGlyph.getPhysicalInstaceByCombined(draggedEUCombinedStr);
							physicalInstanceId = Int32.Parse(physInstId);
							m_Shelf.removePhysicalEUInstance(factoryIndex, euIndex, physicalInstanceId);
						}
					}

                    try
                    {
                        Int32 hWnd = (Int32) getWindowHwnd();
						AudioEngineWrapper.getDefault()._setHwnd((int)hWnd);

                        CurrentAlgorithm.addElaborationUnitGlyph(p.X, p.Y, euDescr, fact, physicalInstanceId);
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

        private AlgorithmGraph m_CurrentAlgorithGraph;
		private AlgorithmGraph CurrentAlgorithm
		{
			set
			{
				m_CurrentAlgorithGraph = value;
				facilities.setCurrentChangeAlgorithm(m_CurrentAlgorithGraph);
			}
			get
			{
				return m_CurrentAlgorithGraph;
			}
		}

        private bool m_IsConnecting = false;
        private String m_SourceEUStringDescr = null;
        //private Polyline m_CurrentConnection;
        private EUConnection m_CurrentConnection;

        private void activateConnection(bool enable)
        {
            m_CurrentConnection.setTransparent(!enable);
        }

        private Point m_ConnectionStartPoint;

        private void RightPanel_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed && !m_IsConnecting)
            {
                m_IsConnecting = true;
                if (e.Source is Shape)
                {
                    String name = (e.Source as Shape).Name;
                    int euIndex = InputOutputGlyph.getEUAudioEngineIdByCombined(name);
					ElaborationUnitGlyphInstance euSrc = CurrentAlgorithm.getEUInstanceByAudioEngineId(euIndex);
                    int portIndex = InputOutputGlyph.getPortIndexByCombined(name);
                    m_CurrentConnection.setSourceEU(euSrc, portIndex);
                    m_SourceEUStringDescr = name;
                    m_ConnectionStartPoint = e.GetPosition(this.RightPanel);
                    PointCollection coll = new PointCollection();
                    coll.Add(m_ConnectionStartPoint);
                    activateConnection(true);
                    m_CurrentConnection.StartPoint = m_ConnectionStartPoint;
                }
            }
        }

        //private String lastFoundEllispeName = "";

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

        private class PointReader
        {

        }

        private static bool ioConnectionFound = false;
        private static Shape m_CurrentIOShape;

        private void resetIOSearch()
        {
            ioConnectionFound = false;
            m_CurrentIOShape = null;
        }

        private void RightPanel_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (m_SourceEUStringDescr != null)
            {
                if (m_IsConnecting)
                {
                    Point p = e.GetPosition((UIElement)sender);
                    resetIOSearch();
                    VisualTreeHelper.HitTest(this.RightPanel,null,new HitTestResultCallback(testResultCallback),
                        new PointHitTestParameters(p));

                    if(ioConnectionFound)
                    {

                        String dstName = m_CurrentIOShape.Name;
                        m_CurrentConnection.EndPoint = p;
                        if(InputOutputGlyph.areIOCompatible(m_SourceEUStringDescr, dstName))
                        {
                            EUConnection conn = new EUConnection(m_CurrentConnection);
                            int euIndex = InputOutputGlyph.getEUAudioEngineIdByCombined(dstName);
							ElaborationUnitGlyphInstance euDst = CurrentAlgorithm.getEUInstanceByAudioEngineId(euIndex);
                            int portIndex = InputOutputGlyph.getPortIndexByCombined(dstName);
                            conn.setDestinationEU(euDst, portIndex);
							CurrentAlgorithm.addConnection(conn);
                            resetIOSearch();
                        }
                    }
                }
            }
            m_SourceEUStringDescr = null;
            m_IsConnecting = false;
            activateConnection(false);
        }


        private void RightPanel_PreviewMouseMove(object sender, MouseEventArgs e)
        {
            if (m_IsConnecting)
            {
                Point endPoint = e.GetPosition(RightPanel);
                m_CurrentConnection.EndPoint = endPoint;
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
			AudioEngineWrapper.getDefault().release();
			StoreSettings();
            App.Current.Shutdown();
        }

        private void MenuItem_Click_1(object sender, RoutedEventArgs e)
        {
            options = new Options();
            options.Owner = this;
            options.LoadAtStartup = Properties.Settings.Default.LoadAtStartUp;
            options.ShowDialog();
            Properties.Settings.Default.LoadAtStartUp = options.LoadAtStartup;
        }

        private bool loadAtStratupLastUseedSynth = false;
        public bool LoadAtStratupLastUseedSynth
        {
            set
            {
                loadAtStratupLastUseedSynth = value;
            }
            get
            {
                return loadAtStratupLastUseedSynth;
            }
        }

        private string loadAtStartUpPath = "";

        private void RetrieveSettings()
        {
            loadAtStratupLastUseedSynth = Properties.Settings.Default.LoadAtStartUp;
            loadAtStartUpPath = Properties.Settings.Default.LoadAtStartUpPath;
        }

        private void StoreSettings()
        {
            //Properties.Settings.Default.LoadAtStartUp = LoadAtStratupLastUseedSynth;
            //Properties.Settings.Default.LoadAtStartUpPath = loadAtStartUpPath;

			Properties.Settings.Default.Save();
        }

		private void Window_Loaded(object sender, RoutedEventArgs e)
		{
			LoadLastSynth();
		}

		private void DockPanel_KeyDown(object sender, KeyEventArgs e)
		{
			int i = 0;
		}

		private void SynthPanelArea_KeyDown(object sender, KeyEventArgs e)
		{
			int i = 0;
		}

		private void Window_KeyDown(object sender, KeyEventArgs e)
		{
			byte[] result = AudioEngineWrapper.getDefault().FirePCKeyboardEvent(e);
			if(result==null)
			{
				
			}
		}

		private void Window_KeyUp(object sender, KeyEventArgs e)
		{
			byte[] result = AudioEngineWrapper.getDefault().FirePCKeyboardEvent(e);
			if (result == null)
			{

			}

		}

		private void PlayButton_Click(object sender, RoutedEventArgs e)
		{
			int res = AudioEngineWrapper.getDefault().PlayAlgorithm(CurrentAlgorithm.algoId);
			if(res!=0)
			{
				MessageBox.Show("Error while playing algorithm: " + res);
				return;
			}
			StopButton.IsChecked = false;
		}

		private void StopButton_Click(object sender, RoutedEventArgs e)
		{
			int res = AudioEngineWrapper.getDefault().StopAlgorithm(CurrentAlgorithm.algoId);
			if(res!=0)
			{
				MessageBox.Show("Error while stopping algorithm: " + res);
				return;
			}
			PlayButton.IsChecked = false;
		}

    }
}

