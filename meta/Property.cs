
// Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

using System;
using System.Collections.Generic;

namespace meta
{
	public class Property
	{
		public string type;
		public string name;

		public bool staticProperty = false;
		public bool read = true;
		public bool write = true;

		public string getter;
		public string setter;

		public void SetName(string newName)
		{
			name = newName;
			if (getter == null)
				getter = newName;
			if (setter == null)
				setter = "set" + newName.ToUpper()[0] + newName.Substring(1) + ':';
		}
	}
}
