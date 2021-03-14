
// Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

using System;
using System.Collections.Generic;

namespace meta
{
	public class Method
	{
		public bool staticMethod;
		public string returnType;
		public string name;
		public string fullName = "";
		public List<string> paramType = new List<string>();
		public List<string> paramName = new List<string>();

		public Method(bool staticMethod)
		{
			this.staticMethod = staticMethod;
		}
	}
}
