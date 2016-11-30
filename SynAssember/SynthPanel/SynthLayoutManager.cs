using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SynthPanels
{
	/**
	 * Manage the layout of the synth panels displayed
	 */

	class SynthLayoutManager
	{
		class SynthPanelWrapper
		{
			public SynthPanelWrapper(ISynthPanel panel)
			{
				m_SynthPanel = panel;
			}
			private ISynthPanel m_SynthPanel = null;
			bool ContainsSynthPanel()
			{
				if (m_SynthPanel == null)
					return false;
				else
					return true;
			}
		}

		public SynthLayoutManager()
		{
			Init();
		}


		public const int MAX_WIDTH = 4;
		public const int MAX_EIGHT = 8;

		SynthPanelWrapper[,] m_PanelMatrix = new SynthPanelWrapper[MAX_WIDTH,MAX_EIGHT];

		private int m_FirstFree_X = 0;
		private int m_FirstFree_Y = 0;

		public void Init()
		{
			m_FirstFree_X = 0;
			m_FirstFree_Y = 0;
			for (int i = 0; i < MAX_WIDTH, i++)
				for (j = 0; j < MAX_EIGHT; j++)
					m_PanelMatrix[i, j] = null;
		}

		const String UNALLOCABLE_SYNTH = "Unable to allocate Synth";

		public void AllocSynthPanel(ISynthPanel panel)
		{
			SynthPanelWrapper synWrapper = new SynthPanelWrapper(panel);
			int xSpan = panel.GetHorizontalSpan();
			int ySpan = panel.GetVerticalSpan();
			// Fits width?
			if(CanAllocateWidth(m_FirstFree_X, m_FirstFree_Y,xSpan, ySpan))
			{
				Fill(m_FirstFree_X, m_FirstFree_Y, xSpan, ySpan, synWrapper);
			}
			else
			{
				//Try next line
				int remainingLines = MAX_WIDTH - m_FirstFree_Y;
				if (remainingLines == 0)
					throw new Exception(UNALLOCABLE_SYNTH);
				else
				{
					//Try to go on each Line
					for(int j=0;j<remainingLines;j++)
					{
						for (int i = 0; i < MAX_WIDTH; i++)
						{
							if (CanAllocateWidth(i, m_FirstFree_Y + j, xSpan, ySpan))
							{
								Fill(i, m_FirstFree_Y + j, xSpan, ySpan, synWrapper);
								return;
							}
						}
					}
					throw new Exception(UNALLOCABLE_SYNTH);
				}
			}
		}

		bool CanAllocateWidth(int xPos, int yPos, int hSpan, int ySpan)
		{
			if (xPos + hSpan > MAX_WIDTH)
				return false;
			for (int i=xPos;i<xPos+hSpan;i++)
				for(int j=yPos;j<yPos+ySpan;j++)
				{
					if (m_PanelMatrix[i, j] != null)
						return false;
				}

			return true;
		}
		
		void Fill(int xPos, int yPos, int hSpan, int ySpan, SynthPanelWrapper panel)
		{
			for (int i = xPos; i < xPos + hSpan; i++)
				for (int j = yPos; j < yPos + ySpan; j++)
					m_PanelMatrix[i, j] = panel;
		}
	}
}
