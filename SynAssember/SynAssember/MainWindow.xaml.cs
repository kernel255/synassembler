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
using System.Windows.Threading;
using EUShelves;
using Microsoft.Win32;
using System.Xml;
using System.Windows.Interop;
using System.IO;
using System.Diagnostics;
using ClayAudioEngine;
using SynthPanels;
using GeneralUtils;
using ClayAudioEngine.Layout;

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

		void CleanUpLayout()
		{
			LeftPanel.Children.Clear();
			RightPanel.Children.Clear();
		}

		AlgorithmPanel m_AlgorithmPanel;
		ShelvesPanel m_ShelvesPanel;

		void InitLayout()
		{
			m_Shelf = new EUShelf(10, 800, 200, 0, 0, AudioEngineWrapper.getDefault().getFactories());
			m_Shelf.addToPanel(LeftPanel);
			m_AlgorithmPanel = new AlgorithmPanel(RightPanel, m_Shelf);
			currentAlgorithm = new AlgorithmGraph(RightPanel);
			m_ShelvesPanel = new ShelvesPanel(LeftPanel);
		}

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
				InitLayout();
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
			e.CanExecute = currentAlgorithm.hasChanged();
        }

		private void saveExecuted(object sender, ExecutedRoutedEventArgs e)
		{
			if(CurrentSynthFilename==null || CurrentSynthFilename.Length==0)
			{
				saveAsExecute(sender, e);
			}
			else
			{
				try
				{
					currentAlgorithm.write(CurrentSynthFilename);
				}
				catch (Exception ex)
				{
					string msg = "Error while saving algorithm:\n" + ex.Message;
					MessageBox.Show(msg);
					return;
				}
			}
		}

		private void canSaveAsExecute(object sender, CanExecuteRoutedEventArgs e)
		{
			e.CanExecute = true;
		}

		private void saveAsExecute(object sender, ExecutedRoutedEventArgs e)
		{
			SaveFileDialog dlg = new SaveFileDialog();
			dlg.Title = "Save a Synthesizer";
			dlg.ShowDialog(this);
			String filename = dlg.FileName;
			if (filename.Length != 0)
			{
				try
				{
					currentAlgorithm.write(filename);
					CurrentSynthFilename = filename;
				}
				catch(Exception ex)
				{
					string msg = "Error while saving algorithm:\n" + ex.Message;
					MessageBox.Show(msg);
					return;
				}
				MessageBox.Show("File saved successfully.");
			}
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
				currentAlgorithm.Dispose();
				CleanUpLayout();
				InitLayout();
				SynthPanelManager.getDefault().CleanUp();
                XmlTextReader reader = new XmlTextReader(dlg.FileName);
				currentAlgorithm = new AlgorithmGraph(RightPanel);
				//facilities.setCurrentChangeAlgorithm(m_CurrentAlgorithGraph);
                Int32 hWnd = (Int32)getWindowHwnd();
				AudioEngineWrapper.getDefault()._setHwnd((int)hWnd);
                try {
					currentAlgorithm.read(reader);
					currentAlgorithm.updateSynthPanels();
					CurrentSynthFilename = dlg.FileName;
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error while reading " + dlg.FileName + "\n" + ex.Message);
                    return;
                }

                Properties.Settings.Default.LoadAtStartUpPath = dlg.FileName;
            }
        }

		bool lastSynthLoaded = false;

		void updateLastSynth()
		{
			if(lastSynthLoaded)
				currentAlgorithm.updateSynthPanels();
		}

		private void LoadLastSynth()
		{
			if (Properties.Settings.Default.LoadAtStartUp == true)
			{
				string filename = Properties.Settings.Default.LoadAtStartUpPath;
				if (filename != null && filename.Length > 0)
				{
					XmlTextReader reader = new XmlTextReader(filename);
					//currentAlgorithm = new AlgorithmGraph(RightPanel);
					Int32 hWnd = (Int32)getWindowHwnd();
					AudioEngineWrapper.getDefault()._setHwnd((int)hWnd);
					try
					{
						currentAlgorithm.read(reader);
						lastSynthLoaded = true;
						CurrentSynthFilename = filename;
						currentAlgorithm.Changed = false;
					}
					catch (Exception ex)
					{
						string msg = "Error while reading: " + filename + "\n" + ex.Message;
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
			currentAlgorithm.Dispose();
			CleanUpLayout();
			InitLayout();
			SynthPanelManager.getDefault().CleanUp();
        }

        private bool initAudioEngine(string applicationFolder)
        {
            StringBuilder dllPath = new StringBuilder(applicationFolder);
			ClayAudioEngine.AudioEngineWrapper.getDefault().init(dllPath, 0, 48000, 16, 1, facilities);

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
			Int32 hWnd = (Int32)getWindowHwnd();
			try
			{
				m_AlgorithmPanel.Drop(sender, e, hWnd);
			}
			catch(Exception ex)
			{
				MessageBox.Show(this, ex.Message);
			}
        }

        private AlgorithmGraph m_CurrentAlgorithGraph;
		private AlgorithmGraph currentAlgorithm
		{
			set
			{
				m_CurrentAlgorithGraph = value;
				facilities.ChangedAlgorithm = m_CurrentAlgorithGraph;
				m_AlgorithmPanel.setCurrentAlgorithmGraph(m_CurrentAlgorithGraph);
			}
			get
			{
				return m_CurrentAlgorithGraph;
			}
		}
        //private Polyline m_CurrentConnection;

        private void activateConnection(bool enable)
        {
			m_AlgorithmPanel.activateConnection(enable);
        }

        private void RightPanel_MouseDown(object sender, MouseButtonEventArgs e)
        {
			m_AlgorithmPanel.MouseDown(sender, e);
        }


        private class PointReader
        {

        }

        private void RightPanel_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
			m_AlgorithmPanel.MouseLeftButtonUp(sender, e);
        }


        private void RightPanel_PreviewMouseMove(object sender, MouseEventArgs e)
        {
			m_AlgorithmPanel.PreviewMouseMove(sender, e);
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
			if(currentAlgorithm.Changed)
			{
				MessageBoxResult res = MessageBox.Show(this, "The Synthesizer has been changed.\nDo you want to save it?", "Save Alert", MessageBoxButton.YesNo);
				if(res.Equals(MessageBoxResult.Yes))
				{
					saveAsExecute(this, null);
				}
			}
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

			Dispatcher.BeginInvoke(
				new Action(()=>
					{
						updateLastSynth();
						currentAlgorithm.Changed = false;
						if(lastSynthLoaded)
						{
							PlayCurrentAlgorithm();
						}
					}
					),
				DispatcherPriority.ContextIdle,
				null);

			SynthPanelArea.Width = SynthPanelContainer.Width;
			SynthPanelArea.Height = SynthPanelContainer.Height;
		}

		private void DockPanel_KeyDown(object sender, KeyEventArgs e)
		{
		}

		private void SynthPanelArea_KeyDown(object sender, KeyEventArgs e)
		{
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

		private void PlayCurrentAlgorithm()
		{
			int res = AudioEngineWrapper.getDefault().PlayAlgorithm(currentAlgorithm.algoId);
			if (res != 0)
			{
				MessageBox.Show("Error while playing algorithm: " + res);
				return;
			}
			PlayButton.IsEnabled = false;
			StopButton.IsEnabled = true;
			StopButton.IsChecked = false;
		}

		private void PlayButton_Click(object sender, RoutedEventArgs e)
		{
			PlayCurrentAlgorithm();
		}

		private void StopButton_Click(object sender, RoutedEventArgs e)
		{
			int res = AudioEngineWrapper.getDefault().StopAlgorithm(currentAlgorithm.algoId);
			if(res!=0)
			{
				MessageBox.Show("Error while stopping algorithm: " + res);
				return;
			}
			PlayButton.IsEnabled = true;
			StopButton.IsEnabled = false;
			PlayButton.IsChecked = false;
		}

		private string _m_CurrentSynthFilename;
		private string CurrentSynthFilename
		{
			get
			{
				return _m_CurrentSynthFilename;
			}
			set
			{
				_m_CurrentSynthFilename = value;
				this.Title = WINDOW_TITLE + value;
			}
		}

		const String WINDOW_TITLE = "SynAssembler - ";

		private void SynthPanelContainer_SizeChanged(object sender, SizeChangedEventArgs e)
		{
			m_AlgorithmPanel.SizeChanged(sender, e);
		}

		private void LeftPanelContainer_SizeChanged(object sender, SizeChangedEventArgs e)
		{
			m_ShelvesPanel.SizeChanged(sender, e);
		}
	}
}

