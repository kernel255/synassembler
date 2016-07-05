using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Shapes;
using System.Windows.Controls;
using System.Windows.Media;
using ClayAudioEngine;

namespace EUShelves
{
    class EULedge
    {
        private class EUGlyphStack
        {
            private double m_X;
            private double m_Y;

            public EUGlyphStack(double x, double y)
            {
                m_X = x;
                m_Y = y;
            }

            private List<ElaborationUnitGlyph> m_Stack = new List<ElaborationUnitGlyph>();

            public void push(ElaborationUnitGlyph glyph)
            {
                m_Stack.Add(glyph);
            }

            public ElaborationUnitGlyph pop()
            {
                if (m_Stack.Count == 0)
                    return null;
                else
                {
                    ElaborationUnitGlyph glyph = m_Stack[0];
                    m_Stack.Remove(glyph);
                    return glyph;
                }
            }

            public int Count
            {
                get { return m_Stack.Count; }
            }

            public ElaborationUnitGlyph At(int index)
            {
                return m_Stack[index];
            }
        }

        private static double SPACE_BETWEEN_EU = 10;

        private double m_Width;
        // This is the height of the ledge itself
        private static double m_Height = 10;
        // This is the space available over the ledge
        private double m_RoomHeight;
        Rectangle m_BaseRect;
        ElaborationUnitFactory m_Factory;
        List<EUGlyphStack> m_Stacks = new List<EUGlyphStack>();
        double m_X, m_Y;
        Canvas m_Panel;

        public double TotalHeight
        {
            get { return m_Height + m_RoomHeight; }
        }

        public static double Height
        {
            get { return m_Height; }
        }

        private List<ElaborationUnitGlyph> euGlyphs = new List<ElaborationUnitGlyph>();

		public EULedge(double width, double room, double x, double y, Brush brush)
		{
			m_X = x;
			m_Y = y;

			m_Width = width;
			m_BaseRect = new Rectangle();
			m_BaseRect.Width = m_Width;
			m_BaseRect.Height = m_Height;
			m_BaseRect.Fill = brush;
			m_RoomHeight = room;
			Canvas.SetTop(m_BaseRect, y);
			Canvas.SetLeft(m_BaseRect, x);
		}


		/**
         * Build a EULedge, one EULedge represent a factory
         */
		public EULedge(double width, double room, double x, double y, ElaborationUnitFactory factory, Brush brush) : this(width, room, x, y, brush)
		{

			int numEU = factory.getEUList().Count;
            m_Factory = factory;

            // Fill the Ledge with the available EU
            for (int i = 0; i < numEU; i++)
            {
                ElaborationUnitDescription euDescr = factory.getEUList().ElementAt(i);
                ElaborationUnitGlyph euGlyph;
                if (euDescr.Physical)
                {
                    EUGlyphStack stack = new EUGlyphStack(getXByEUIndex(i), getYByPhysIndex(0));
                    m_Stacks.Add(stack);
                    IList<ElaborationUnitPhysicalInstance> insts = euDescr.getInstances();
                    if (insts.Count == 0)
                    {
                        
                        //euGlyph = new ElaborationUnitGlyph(getXByEUIndex(i), m_Y - ElaborationUnitGlyph.Height, euDescr, factory);
                        euGlyph = new ElaborationUnitGlyph(getXByEUIndex(i), getYByPhysIndex(0), euDescr, factory, false);
                        euGlyph.setUnavailable();
                        euGlyphs.Add(euGlyph);
                    }
                    else
                    {
                        double localHeight = 0;
                        for (int physicalInstanceIndex = 0; physicalInstanceIndex < insts.Count; physicalInstanceIndex++)
                        {
                            ElaborationUnitPhysicalInstance inst = insts.ElementAt(physicalInstanceIndex);
                            string instName = inst.m_Name;
                            euGlyph = new ElaborationUnitGlyph(getXByEUIndex(i), getYByPhysIndex(physicalInstanceIndex), euDescr, factory, instName, physicalInstanceIndex, false);
                            localHeight += ElaborationUnitGlyph.Height;
                            euGlyphs.Add(euGlyph);
                            stack.push(euGlyph);
                        }
                    }
                }
                else
                {
                    euGlyph = new ElaborationUnitGlyph(getXByEUIndex(i), getYByPhysIndex(0), euDescr, factory, true);
                    euGlyphs.Add(euGlyph);
                }
                
                //pos += SPACE_BETWEEN_EU + ElaborationUnitGlyph.Width;
            }

        }

        private double getXByEUIndex(int index)
        {
            return m_X + index * (ElaborationUnitGlyph.Width + SPACE_BETWEEN_EU);
        }

        private double getYByPhysIndex(int index)
        {
            return m_Y - ElaborationUnitGlyph.Height - (index * ElaborationUnitGlyph.Height);
        }

        public void addToPanel(Canvas panel)
        {
            m_Panel = panel;
			if (m_Factory != null)
			{
				int numEU = m_Factory.getEUList().Count;
				for (int i = 0; i < numEU; i++)
					//euGlyphs[i].addToPanel(m_Panel);
					euGlyphs[i].addToCanvas(m_Panel);
			}
            m_Panel.Children.Add(m_BaseRect);
        }

        public void removePhysicalEUGlyph(int euIndex, int physInstanceIndex)
        {
            // Remove the instanced instance
            EUGlyphStack stack = m_Stacks[euIndex];
            ElaborationUnitGlyph removedGlyph = stack.At(physInstanceIndex);
            removedGlyph.removeFromPanel(m_Panel);
            //Slide all the other EU
            int totalEU = stack.Count;
            int toBeSlidedEU = totalEU - (physInstanceIndex + 1);
            if (toBeSlidedEU > 0)
            {
                double distance = ElaborationUnitGlyph.Height * (physInstanceIndex + 1);
                ElaborationUnitGlyph[] movedEUs = new ElaborationUnitGlyph[toBeSlidedEU];
                for (int i = 0; i < toBeSlidedEU; i++)
                {
                    movedEUs[i] = stack.At(physInstanceIndex + 1 + i);
                }
                int steps = 200;
                double step = distance / steps;
                double targetY = physInstanceIndex * ElaborationUnitGlyph.Height;
                for (int i = 0; i < steps; i++)
                {
                    foreach(ElaborationUnitGlyph eu in movedEUs)
                    {
                        eu.moveYBy(step);
                    }
                }
            }
        }
    }
}
