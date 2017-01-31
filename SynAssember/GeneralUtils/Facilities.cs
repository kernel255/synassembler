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
		private String _resourcesFolder;
		public String ResourcesFolder
		{
			set { _resourcesFolder = value; }
			get { return _resourcesFolder; }
		}

		public static void Log(String logLine)
		{
			Console.WriteLine(logLine);
		}

		public static void Log(string format, params object[] arg)
		{
			Console.WriteLine(format, arg);
		}

	}

}

