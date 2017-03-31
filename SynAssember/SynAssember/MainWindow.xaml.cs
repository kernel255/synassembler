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
	/// 

	internal static class NativeMethods
	{
		[DllImport("kernel32.dll", ExactSpelling = true)]
		internal static extern IntPtr GetConsoleWindow();

	}

	public partial class MainWindow : Window
    {
		/*
        [DllImport("kernel32.dll", ExactSpelling = true)]
        private static extern IntPtr GetConsoleWindow();
		*/

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

		Options options;

		Facilities facilities;

		void CleanUpLayout()
		{
			m_ShelvesPanel.CleanUp();
			m_AlgorithmPanel.CleanUp();
		}

		AlgorithmPanel m_AlgorithmPanel;
		ShelvesPanel m_ShelvesPanel;

		void InitLayout()
		{
			m_ShelvesPanel = new ShelvesPanel(LeftPanel);
			m_AlgorithmPanel = new AlgorithmPanel(RightPanel, m_ShelvesPanel.Shelf);
			currentAlgorithm = new AlgorithmGraph(RightPanel);
		}

		const String RESOURCES_FOLDER_NAME = "Resources";

		public MainWindow()
		{
			InitializeComponent();
			fileManagementBindings();
			facilities = new Facilities();
			String currentFolder = Directory.GetCurrentDirectory();
			Trace.Write("Current folder=" + currentFolder);
			facilities.ResourcesFolder = currentFolder + "\\" + RESOURCES_FOLDER_NAME;

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
			dlg.Filter = SYNASSEMBLER_EXTENSION_DIALOG;
			dlg.DefaultExt = SYNASSEMBLER_EXTENSION;
			dlg.AddExtension = true;
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
			dlg.Filter = SYNASSEMBLER_EXTENSION_DIALOG;
            dlg.ShowDialog(this);
            if(dlg.FileName.Length!=0)
            {
				currentAlgorithm.Dispose();
				CleanUpLayout();
				InitLayout();
				SynthPanelManager.getDefault().CleanUp();
                XmlTextReader reader = new XmlTextReader(dlg.FileName);
				currentAlgorithm = new AlgorithmGraph(RightPanel);
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

        private void LeftPanel_PreviewMouseDown(object sender, MouseButtonEventArgs e) { m_ShelvesPanel.PreviewButtonDown(e); }
        private void LeftPanel_PreviewMouseMove(object sender, MouseEventArgs e) { m_ShelvesPanel.PreviewMouseMove(sender, e); }

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

        private void activateConnection(bool enable) { m_AlgorithmPanel.activateConnection(enable); }
        private void RightPanel_MouseDown(object sender, MouseButtonEventArgs e) { m_AlgorithmPanel.MouseDown(sender, e); }
        private void RightPanel_MouseLeftButtonUp(object sender, MouseButtonEventArgs e) { m_AlgorithmPanel.MouseLeftButtonUp(sender, e); }
        private void RightPanel_PreviewMouseMove(object sender, MouseEventArgs e) {	m_AlgorithmPanel.PreviewMouseMove(sender, e); }

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
			options.HardcodedMode = Properties.Settings.Default.HardcodedSynth;
            options.ShowDialog();
            Properties.Settings.Default.LoadAtStartUp = options.LoadAtStartup;
			Properties.Settings.Default.HardcodedSynth = options.HardcodedMode;
			if(options.OkSelected)
			{
				HideShelves(options.HardcodedMode);
			}
			
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

		private bool hardcodedSynth = false;
		public bool HardCodedSynth
		{
			set { hardcodedSynth = value; }
			get { return hardcodedSynth; }
		}

        private string loadAtStartUpPath = "";

        private void RetrieveSettings()
        {
            loadAtStratupLastUseedSynth = Properties.Settings.Default.LoadAtStartUp;
            loadAtStartUpPath = Properties.Settings.Default.LoadAtStartUpPath;
			hardcodedSynth = Properties.Settings.Default.HardcodedSynth;
        }

        private void StoreSettings()
        {
			Properties.Settings.Default.Save();
        }

		private void HideShelves(bool hide)
		{
			if (hide)
				MainGrid.RowDefinitions[0].Height = new GridLength(0);
			else
				MainGrid.RowDefinitions[0].Height = new GridLength(300);
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void Window_Loaded(object sender, RoutedEventArgs e)
		{
			LoadLastSynth();

			if(Properties.Settings.Default.HardcodedSynth)
			{
				HideShelves(true);
			}



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
		const String SYNASSEMBLER_EXTENSION_DIALOG = "SynAssembler Synthesizer (sya)|*sya";
		const String SYNASSEMBLER_EXTENSION = "sya";

		private void SynthPanelContainer_SizeChanged(object sender, SizeChangedEventArgs e)	{ SynthPanelManager.getDefault().SizeChanged(sender, e); }
		private void LeftPanelContainer_SizeChanged(object sender, SizeChangedEventArgs e) { m_ShelvesPanel.SizeChanged(sender, e);	}
		private void RightPanelContainer_SizeChanged(object sender, SizeChangedEventArgs e)	{ m_AlgorithmPanel.SizeChanged(sender, e); }

	}
}
