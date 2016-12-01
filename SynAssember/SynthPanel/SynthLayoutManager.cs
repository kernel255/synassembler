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

			ISynthPanel SynthPanel
			{
				get { return m_SynthPanel; }
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

		class AllocablePosition
		{
			public bool allocable;
			public int i;
			public int j;
			public int nextAllocablei;
			public int nextAllocablej;
		}

		AllocablePosition CalculateAllocablePosition(int xPos, int yPos, int hSpan, int vSpan)
		{
			AllocablePosition result = new AllocablePosition();
			// Fits width?
			if (CanAllocateWidth(m_FirstFree_X, m_FirstFree_Y, hSpan, vSpan))
			{
				result.i = m_FirstFree_X;
				result.j = m_FirstFree_Y;
				result.nextAllocablei = m_FirstFree_X + hSpan;
				result.nextAllocablej = m_FirstFree_Y;
				result.allocable = true;
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
					for (int j = 0; j < remainingLines; j++)
					{
						for (int i = 0; i < MAX_WIDTH; i++)
						{
							if (CanAllocateWidth(i, m_FirstFree_Y + j, hSpan, vSpan))
							{
								result.i = i;
								result.j = m_FirstFree_Y+j;
								result.nextAllocablei = i + hSpan;
								result.nextAllocablej = m_FirstFree_Y + j;
								result.allocable = true;
								return result;
							}
						}
					}
					result.allocable = false;
				}
			}

			return result;
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
			for (int i = 0; i < MAX_WIDTH; i++)
				for (int j = 0; j < MAX_EIGHT; j++)
					m_PanelMatrix[i, j] = null;
		}

		const String UNALLOCABLE_SYNTH = "Unable to allocate Synth";

		public void AllocSynthPanel(ISynthPanel panel)
		{
			SynthPanelWrapper synWrapper = new SynthPanelWrapper(panel);
			int xSpan = panel.GetHorizontalSpan();
			int ySpan = panel.GetVerticalSpan();

			AllocablePosition allocablePos = CalculateAllocablePosition(m_FirstFree_X, m_FirstFree_Y, xSpan, ySpan);
			if(!allocablePos.allocable)
				throw new Exception(UNALLOCABLE_SYNTH);
			Fill(allocablePos.i, allocablePos.j, xSpan, ySpan, synWrapper);

			int xPos = (allocablePos.i + 1) * SPACE_X_BETWEENPANELS + allocablePos.i * BASE_WIDTH_UNIT;
			int yPos = (allocablePos.j + 1) * SPACE_Y_BETWEENPANELS + allocablePos.j * BASE_HEIGHT_UNIT;
			panel.x = xPos;
			panel.y = yPos;


			m_FirstFree_X = allocablePos.nextAllocablei;
			m_FirstFree_Y = allocablePos.nextAllocablej;





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

		internal const int SPACE_X_BETWEENPANELS = 10;
		internal const int SPACE_Y_BETWEENPANELS = 7;
		const int BASE_WIDTH_UNIT = 150;
		const int BASE_HEIGHT_UNIT = 100;

	}
}
