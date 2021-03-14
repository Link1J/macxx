
// Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

using System;

namespace meta
{
	public class Token
	{
		string file;
		int start;
		int length;
		public Parser parser;
		Lazy<Token> _next;
		Token _last;

		public Token Next
		{
			get
			{
				return _next.Value;
			}
		}

		public Token Prev
		{
			get
			{
				return _last;
			}
		}

		public bool Valid
		{
			get
			{
				return file != null;
			}
		}

		public bool isSymbol { get; private set; }

		public Token(Parser parser, string file, int start, int length, bool isSymbol)
		{
			this.file = file;
			this.start = start;
			this.length = length;
			this.isSymbol = isSymbol;
			this.parser = parser;

			_last = parser.Previous;
			_next = new Lazy<Token>(() =>
			{
				parser.Next();
				return parser.Current;
			});
		}

		public Token(Parser parser)
		{
			this.start = -1;
			this.length = -1;
			this.parser = parser;

			_last = parser.Previous;
			_next = new Lazy<Token>(() =>
			{
				return this;
			});
		}

		public string Text
		{
			get
			{
				return file.Substring(start, length);
			}
		}
	}
}
