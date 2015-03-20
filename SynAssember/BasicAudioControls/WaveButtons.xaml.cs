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

namespace BasicAudioControls
{
    

    /// <summary>
    /// Interaction logic for UserControl1.xaml
    /// </summary>
    public partial class WaveButton : UserControl
    {
        public static DependencyProperty SelectedWaveProperty;

        public String SelectedWave
        {
            get { return (String)GetValue(SelectedWaveProperty); }
            set { SetValue(SelectedWaveProperty, value); }
        }


        static WaveButton()
        {
            SelectedWaveProperty = DependencyProperty.Register("SelectedWave", typeof(Int32), typeof(BasicSlider),
                new FrameworkPropertyMetadata(0, new PropertyChangedCallback(OnSelectedWaveChanged)));
        }

        public WaveButton()
        {
            InitializeComponent();
        }

        private static void OnSelectedWaveChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
        {
        }

        private void SawtoothWave_Checked(object sender, RoutedEventArgs e)
        {
            TriangleWave.IsChecked = false;
            SineWave.IsChecked = false;
            SquareWave.IsChecked = false;
        }
        private void SineWaveWave_Checked(object sender, RoutedEventArgs e)
        {
            TriangleWave.IsChecked = false;
            SawtoothWave.IsChecked = false;
            SquareWave.IsChecked = false;
        }

        private void SquareWave_Checked(object sender, RoutedEventArgs e)
        {
            TriangleWave.IsChecked = false;
            SawtoothWave.IsChecked = false;
            SineWave.IsChecked = false;
        }

        private void TriangleWave_Checked(object sender, RoutedEventArgs e)
        {
            SquareWave.IsChecked = false;
            SawtoothWave.IsChecked = false;
            SineWave.IsChecked = false;
        }
    }
}
