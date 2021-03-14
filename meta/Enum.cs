
// Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

using System;
using System.Collections.Generic;

namespace meta
{
	public class Enum
	{
		public string type;
		public string name;
		public bool option;
		public List<string> members = new List<string>();
		public List<string> values = new List<string>();

		public Enum(bool option)
		{
			this.option = option;
		}
	}
}
