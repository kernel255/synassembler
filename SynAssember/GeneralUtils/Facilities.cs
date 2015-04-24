using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GeneralUtils
{
	public class Facilities
	{
		ChangedAlgoruthmInterface changedAlgorithm;
		public void setCurrentChangeAlgorithm(ChangedAlgoruthmInterface algo)
		{
			changedAlgorithm = algo;
		}
	}
}

