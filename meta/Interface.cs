
// Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

using System;
using System.Collections.Generic;

namespace meta
{
	public class Interface
	{
		public string name;
		public string baseClass;
		public List<string> protocols = new List<string>();
		public List<Method> methods = new List<Method>();
		public List<Property> properties = new List<Property>();

		public Interface(string name)
		{
			this.name = name;
		}
	}
}
