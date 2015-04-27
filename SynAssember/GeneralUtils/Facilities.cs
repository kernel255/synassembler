using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GeneralUtils
{
	public class Facilities
	{
		private ChangedAlgoruthmInterface changedAlgorithm;
		public ChangedAlgoruthmInterface ChangedAlgorithm
		{
			get
			{
				return changedAlgorithm;
			}
			set
			{
				changedAlgorithm = value;
			}
		}
	}
}

