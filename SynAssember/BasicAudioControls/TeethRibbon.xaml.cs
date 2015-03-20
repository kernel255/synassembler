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
    /// Interaction logic for TeethRibbon.xaml
    /// </summary>
    public partial class TeethRibbon : UserControl
    {
        public TeethRibbon()
        {
            InitializeComponent();
        }

        private void Area_Loaded(object sender, RoutedEventArgs e)
        {
            double width = BackgroundRect.ActualWidth / 2;
            double height = BackgroundRect.ActualHeight / 2;
            Polyline outer = DrawUtils.createCrown(width, height, 0.8 * width, 0.7 * width, 10);
            outer.Fill = Brushes.Blue;
            Area.Children.Add(outer);
            Polyline inner = DrawUtils.createCrown(width, height, 0.6 * width, 0.5 * width, 10);
            inner.Fill = Brushes.Red;
            Area.Children.Add(inner);

            IList<Line> ticks = DrawUtils.createTicks(width, height, 0.85 * width, 0.95 * width, 60, 4);
            foreach (Line tick in ticks)
                Area.Children.Add(tick);

        }
    }
}
