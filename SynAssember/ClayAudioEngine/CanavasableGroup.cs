using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Shapes;
using System.Windows.Controls;
using System.Windows;

namespace ClayAudioEngine
{
    public class CanavasableGroup : ICanavasable
    {
        List<UIElement> m_UIElementList = new List<UIElement>();

        public void addToCanvas(Canvas canvas)
        {
            int numShapes = m_UIElementList.Count;
            for (int i = 0; i < numShapes; i++)
            {
                canvas.Children.Add(m_UIElementList[i]);
            }
        }

        public void moveByX(double x)
        {
            int numShapes = m_UIElementList.Count;
            for (int i = 0; i < numShapes; i++)
            {
                UIElement uielement = m_UIElementList[i];
                double curr_x = Canvas.GetLeft(uielement);
                Canvas.SetLeft(uielement, curr_x + x);
            }
        }
        public void moveByY(double y)
        {
            int numShapes = m_UIElementList.Count;
            for (int i = 0; i < numShapes; i++)
            {
                UIElement uielement = m_UIElementList[i];
                double curr_y = Canvas.GetTop(uielement);
                Canvas.SetTop(uielement, curr_y + y);
            }
        }

        public void addShape(UIElement uielement)
        {
            m_UIElementList.Add(uielement);
        }
        public void addGroup(CanavasableGroup group)
        {
            int num = group.m_UIElementList.Count;
            for (int i = 0; i < num; i++)
            {
                UIElement uielement = group.m_UIElementList[i];
                m_UIElementList.Add(uielement);
            }
        }
        
    }
}
