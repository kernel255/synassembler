﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using EUShelves;
using System.Windows.Input;
using System.Windows.Shapes;

namespace ClayAudioEngine.Layout
{
	public class ShelvesPanel
	{

		public ShelvesPanel(Canvas canvas)
		{
			this.canvas = canvas;
			IList<ElaborationUnitFactory> factories = AudioEngineWrapper.getDefault().getFactories();
			if (factories == null)
			{
				MessageBox.Show("Fatal error: no AudioEngine factories available");
				Application.Current.Shutdown();
			}
			m_Shelf = new EUShelf(6, 1000, 200, 0, 0, factories);
			m_Shelf.addToPanel(canvas);
		}

		public void SizeChanged(object sender, SizeChangedEventArgs e)
		{
			try { canvas.Width = GetWidth(); } catch { }
			try { canvas.Height = GetHeight(); } catch { }
		}

		public void CleanUp()
		{
			canvas.Children.Clear();
		}

		double GetWidth()
		{
			return m_Shelf.GetSize().Width;
		}

		double GetHeight()
		{
			return m_Shelf.GetSize().Height;
		}

		Canvas canvas;
		private EUShelf m_Shelf;
		public EUShelf Shelf
		{
			get { return m_Shelf; }
		}

		private bool m_IsDragging = false;
		private Point m_DragStartPoint;
		public Point DragStartPoint
		{
			set { m_DragStartPoint = value; }
			get { return m_DragStartPoint; }
		}

		public void PreviewButtonDown(MouseButtonEventArgs e)
		{
			m_DragStartPoint = e.GetPosition(null);
		}

		public void PreviewMouseMove(object sender, MouseEventArgs e)
		{
			if (e.LeftButton == MouseButtonState.Pressed && !m_IsDragging)
			{
				Point position = e.GetPosition(null);
				if (Math.Abs(position.X - DragStartPoint.X) > SystemParameters.MinimumHorizontalDragDistance ||
									Math.Abs(position.Y - DragStartPoint.Y) > SystemParameters.MinimumVerticalDragDistance)
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
				Rectangle rect = (Rectangle)e.Source;
				String name = rect.Name;
				DataObject data = new DataObject(System.Windows.DataFormats.Text.ToString(), name);
				DragDropEffects de = DragDrop.DoDragDrop(canvas, data, DragDropEffects.Move);
			}
			m_IsDragging = false;
		}

	}
}
