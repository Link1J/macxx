
// Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

using System;
using System.Collections.Generic;

namespace meta
{
	public class Reader
	{
		public List<Interface> interfaces = new List<Interface>();
		public List<Protocol> protocols = new List<Protocol>();
		public List<Enum> enums = new List<Enum>();

		Token Global(Token token)
		{
			while (token.Valid)
			{
				if (token.Text == "#")
				{
					token = Preprocessor(token.Next);
				}
				else if (token.Text == "@")
				{
					token = token.Next;
					if (token.Text == "implementation")
					{
						token = Implementation(token.Next);
					}
					else if (token.Text == "protocol")
					{
						token = Protocol(token.Next);
					}
					else if (token.Text == "interface")
					{
						token = Interface(token.Next);
					}
				}
				else if (token.Text == "NS_ENUM" || token.Text == "NS_OPTIONS")
				{
					token = Enum(token.Next);
				}
				token = token.Next;
			}
			return token;
		}

		Token Preprocessor(Token token)
		{
			while (token.Valid)
			{
				if (token.Text == "\n")
				{
					break;
				}
				token = token.Next;
			}
			return token;
		}

		Token Implementation(Token token)
		{
			while (token.Valid)
			{
				if (token.Text == "\n")
				{
					break;
				}
				token = token.Next;
			}
			return token;
		}

		Token Protocol(Token token)
		{
			if (!protocols.Exists(x => x.name == token.Text))
				protocols.Add(new Protocol(token.Text));
			Protocol new_item = protocols.Find(x => x.name == token.Text);

			while (token.Valid)
			{
				if (token.Text == "\n")
				{
					token = Details(token.Next, new_item);
					break;
				}
				else if (!token.isSymbol)
				{
					new_item.protocols.Add(token.Text);
				}
				else if (token.Text == "{")
				{
					token = Block(token.Next);
				}
				token = token.Next;
			}
			return token;
		}

		Token Interface(Token token)
		{
			if (!interfaces.Exists(x => x.name == token.Text))
				interfaces.Add(new Interface(token.Text));
			Interface new_item = interfaces.Find(x => x.name == token.Text);
			token = token.Next;

			int mode = 0;

			while (token.Valid)
			{
				if (token.Text == "\n")
				{
					token = Details(token.Next, new_item);
					break;
				}
				else if (token.Text == "(")
				{
					mode = 3;
				}
				else if (!token.isSymbol && mode == 0)
				{
					new_item.baseClass = token.Text;
					token = token.Next;

					if (token.Text == "<")
					{
						mode = 2;
					}
					else
					{
						mode = 3;
					}
				}
				else if (token.Text == "<" && mode == 1)
				{
					mode = 2;
				}
				else if (!token.isSymbol && mode == 2)
				{
					new_item.protocols.Add(token.Text);
				}
				else if (token.Text == ">" && mode == 2)
				{
					mode = 3;
				}
				else if (token.Text == "{")
				{
					token = Block(token.Next);
				}
				token = token.Next;
			}
			return token;
		}

		Token Block(Token token)
		{
			while (token.Valid)
			{
				if (token.Text == "}")
				{
					break;
				}
				token = token.Next;
			}
			return token;
		}

		Token Details(Token token, object new_item)
		{
			while (token.Valid)
			{
				if (token.Text == "@")
				{
					token = token.Next;
					if (token.Text == "end")
					{
						break;
					}
					else if (token.Text == "property")
					{
						token = Property(token.Next, new_item);
					}
				}
				else if (token.Text == "+")
				{
					token = Method(token.Next, new_item, true);
				}
				else if (token.Text == "-")
				{
					token = Method(token.Next, new_item, false);
				}
				token = token.Next;
			}
			return token;
		}

		Token Method(Token token, object clazz, bool static_method)
		{
			Method new_item = new Method(static_method);

			if (clazz is Interface inter)
			{
				inter.methods.Add(new_item);
			}
			else if (clazz is Protocol proto)
			{
				proto.methods.Add(new_item);
			}

			int mode = 0;
			string type = "";
			string new_part = "";

			if (token.Text == "(")
			{
				mode = 2;
				token = token.Next;
			}

			while (token.Valid)
			{
				if (token.Text == ";")
				{
					break;
				}
				else if (token.Text == "API_DEPRECATED" || token.Text == "API_AVAILABLE")
				{
					while (token.Valid)
					{
						if (token.Text == "\n")
						{
							break;
						}
						token = token.Next;
					}
				}
				else if (!token.isSymbol && mode == 0)
				{
					if (new_item.name == null)
						new_item.name = token.Text;
					new_part = token.Text;
				}
				else if (token.Text == ":" && mode == 0)
				{
					new_item.fullName += new_part + ":";
					mode = 1;
				}
				else if (token.Text == "(" && mode == 1)
				{
					mode = 2;
				}
				else if (!token.isSymbol && mode == 2)
				{
					type = token.Text;
				}
				else if (token.Text == ")" && mode == 2)
				{
					if (new_item.name == null)
					{
						new_item.returnType = type;
						mode = 0;
					}
					else
					{
						new_item.paramType.Add(type);
						mode = 3;
					}
					type = "";
				}
				else if (token.isSymbol && mode == 2)
				{
					type += token.Text;
				}
				else if (mode == 3)
				{
					new_item.paramName.Add(token.Text);
					mode = 0;
				}
				token = token.Next;
			}

			if (new_item.fullName == "")
				new_item.fullName = new_item.name;

			return token;
		}

		Token Property(Token token, object clazz)
		{
			Property new_item = new Property();

			if (clazz is Interface inter)
			{
				inter.properties.Add(new_item);
			}
			else if (clazz is Protocol proto)
			{
				proto.properties.Add(new_item);
			}

			int mode = 0;
			string type = "";
			string name = "";

			while (token.Valid)
			{
				if (token.Text == ";")
				{
					new_item.type = type;
					new_item.SetName(name);
					break;
				}
				else if (token.Text == "API_DEPRECATED" || token.Text == "API_AVAILABLE")
				{
					int count = 0;
					while (token.Valid)
					{
						if (token.Text == "(")
						{
							count++;
						}
						else if (token.Text == ")")
						{
							count--;
							if (count <= 0)
								break;
						}
						token = token.Next;
					}
				}
				else if (token.Text == "(")
				{
					mode = 1;
				}
				else if (!token.isSymbol && mode == 0)
				{
					type = name;
					name = token.Text;
				}
				else if (token.isSymbol && mode == 0)
				{
					type = name + token.Text;
					mode = 3;
				}
				else if (!token.isSymbol && mode == 3)
				{
					name = token.Text;
				}
				else if (token.isSymbol && mode == 3)
				{
					type = name + token.Text;
				}
				else if (!token.isSymbol && mode == 1)
				{
					if (token.Text == "readonly")
					{
						new_item.write = false;
					}
					else if (token.Text == "getter")
					{
						new_item.getter = token.Next.Next.Text;
						token = token.Next.Next;
					}
					else if (token.Text == "class")
					{
						new_item.staticProperty = true;
					}
				}
				else if (token.Text == ")" && mode == 1)
				{
					mode = 0;
				}
				token = token.Next;
			}
			return token;
		}

		Token Enum(Token token)
		{
			Token type = token.Next;
			Token name = token.Next.Next.Next;

			Enum new_item;
			if (name.Text == "{")
			{
				if (!enums.Exists(x => x.name == "" && x.type == type.Text))
				{
					new_item = new Enum(false);
					new_item.type = type.Text;
					enums.Add(new_item);
				}
				else
				{
					new_item = enums.Find(x => x.name == "" && x.type == type.Text);
				}
				token = name.Next;
			}
			else
			{
				enums.Add(new Enum(token.Prev.Text == "NS_OPTIONS"));
				new_item = enums[enums.Count - 1];
				new_item.name = name.Text;
				new_item.type = type.Text;
				token = name.Next.Next.Next;
			}

			while (name.Next.Text != ")")
			{
				name = name.Next;
			}
			//

			int mode = 0;

			while (token.Valid)
			{
				if (token.Text == "}")
				{
					break;
				}
				else if (token.Text == ",")
				{
					if (new_item.values.Count != new_item.members.Count)
						new_item.values.Add("");
					mode = 0;
				}
				else if (!token.isSymbol && mode == 0)
				{
					new_item.members.Add(token.Text);
					mode = 1;
				}
				else if ((token.Text == "API_DEPRECATED" || token.Text == "API_AVAILABLE") && mode == 1)
				{
					int count = 0;
					while (token.Valid)
					{
						if (token.Text == "(")
						{
							count++;
						}
						else if (token.Text == ")")
						{
							count--;
							if (count <= 0)
								break;
						}
						token = token.Next;
					}
				}
				else if (token.Text == "=" && mode == 1)
				{
					mode = 2;
				}
				else if (mode == 2)
				{
					if (new_item.values.Count != new_item.members.Count)
						new_item.values.Add("");
					new_item.values[new_item.values.Count - 1] += token.Text + " ";
				}
				token = token.Next;
			}
			return token;
		}

		public void Run(Parser parser)
		{
			parser.MoveNext();
			Global(parser.Current);
		}
	}
}
