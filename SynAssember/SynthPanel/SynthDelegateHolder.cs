﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SynthPanels
{
	public struct SynthDelegateHolder
	{
		public WriteEUDoubleProperty writeEUDProp;
		public WriteEUIntegerProperty writeEUIProp;

		public ReadEUDoubleProperty readEUDprop;
		public ReadEUIntegerProperty readEUIProp;

		public WriteEUBoolProperty writeEUBProp;
		public ReadEUBoolProperty readEUBProp;

		public WriteEUProperty writeEUProp;
		public ReadEUProperty readEUProp;
	}
}
