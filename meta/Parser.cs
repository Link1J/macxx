
// Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

using System;
using System.IO;
using System.Collections;

namespace meta
{
	public class Parser : IEnumerable, IEnumerator
	{
		public string filename;
		string text;
		int position = 0;
		Token last;
		Token current;

		public bool AtEnd
		{
			get
			{
				return position >= text.Length;
			}
		}

		bool isSymbol()
		{
			if (AtEnd)
				return false;

			switch (text[position])
			{
				case '@':
				case ',':
				case ':':
				case ';':
				case '+':
				case '-':
				case '*':
				case '#':
				case '=':
				case '"':
				case '^':
				case '(':
				case ')':
				case '[':
				case ']':
				case '<':
				case '>':
				case '{':
				case '}':
				case '\n':
					return true;

				default:
					return false;
			}
		}
		bool isLetter()
		{
			if (AtEnd)
				return false;

			if (isSymbol())
				return false;

			switch (text[position])
			{
				case ' ':
				case '\t':
				case '\r':
					return false;

				default:
					return true;
			}
		}

		void clearWhitespace()
		{
			while (true)
			{
				if ((position + 2) <= text.Length)
				{
					var first_two = text.Substring(position, 2);
					if (first_two == "//")
					{
						position++;
						do
						{
							position++;
							if (AtEnd)
								return;
						} while (text[position] != '\n');
						position++;
					}
					else if (first_two == "/*")
					{
						position++;
						do
						{
							position++;
							if (AtEnd && position + 2 <= text.Length)
								return;
							first_two = text.Substring(position, 2);
						} while (first_two != "*/");
						position += 2;
					}
				}

				if (AtEnd)
					return;

				switch (text[position])
				{
					case ' ':
					case '\t':
					case '\r':
						position++;
						break;

					default:
						return;
				}
			}
		}

		public Parser(string filename)
		{
			this.filename = filename.Substring(filename.LastIndexOfAny(new char[]{'/', '\\'}) + 1);
			text = File.ReadAllText(filename);
		}

		IEnumerator IEnumerable.GetEnumerator()
		{
			return (IEnumerator)this;
		}

		public void Next()
		{
			if (AtEnd && position != text.Length)
				return;

			clearWhitespace();

			last = current;

			if (position == text.Length)
			{
				current = new Token(this);
			}
			else if (isSymbol())
			{
				current = new Token(this, text, position, 1, true);
				position++;
			}
			else
			{
				var start = position;
				while (isLetter())
				{
					position++;
				}
				current = new Token(this, text, start, position - start, false);
			}
		}

		public bool MoveNext()
		{
			if (current != null)
				current = current.Next;
			else
				Next();

			return !AtEnd;
		}

		public void Reset()
		{
			position = 0;
		}

		object IEnumerator.Current
		{
			get
			{
				return Current;
			}
		}

		public Token Current
		{
			get
			{
				return current;
			}
		}

		public Token Previous
		{
			get
			{
				return last;
			}
		}
	}
}
