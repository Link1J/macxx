
// Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

using System;
using System.Collections.Generic;

namespace meta
{
	public class Framework
	{
		public string name;
		public List<Class> interfaces = new List<Class>();
		public List<Enum> enums = new List<Enum>();

		public Framework(string name)
		{
			this.name = name;
		}
	}
}
