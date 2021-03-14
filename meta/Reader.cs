
// Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

using System;
using System.Collections.Generic;

namespace meta
{
	public class Reader
	{
		public List<Framework> frameworks = new List<Framework>();

		List<Class> interfaces;
		List<Enum> enums;

		bool isCleanupTag(string text)
		{
			return text == "NS_SWIFT_UNAVAILABLE"
				|| text == "API_DEPRECATED"
				|| text == "API_AVAILABLE"
				|| text == "NS_SWIFT_NAME"
				|| text == "NS_DEPRECATED"
				|| text == "API_DEPRECATED_WITH_REPLACEMENT"
				|| text == "API_UNAVAILABLE"
				|| text == "NS_DEPRECATED_MAC"
				|| text == "NS_AVAILABLE"
				|| text == "NS_AVAILABLE_IOS"
				|| text == "NS_AVAILABLE_MAC"
				|| text == "__OSX_AVAILABLE_BUT_DEPRECATED_MSG"
				|| text == "__OSX_AVAILABLE_STARTING"
				|| text == "NS_DEPRECATED_IOS"
				|| text == "__TVOS_AVAILABLE"
				|| text == "_NS_SCANNER_DEPRECATED_FOR_SWIFT_ONLY_WITH_REPLACEMENT"
				|| text == "__WATCHOS_AVAILABLE"
				|| text == "IC_AVAILABLE"
				|| text == "IC_DEPRECATED_WITH_REPLACEMENT"
				|| text == "IC_DEPRECATED"
				|| text == "IC_UNAVAILABLE"
				|| text == "DEPRECATED_MSG_ATTRIBUTE"
				|| text == "MP_API"
				|| text == "MP_DEPRECATED"
				|| text == "MP_DEPRECATED_WITH_REPLACEMENT"
				|| text == "MP_UNAVAILABLE"
				|| text == "PDFKIT_AVAILABLE"
				|| text == "PDFKIT_DEPRECATED"
				|| text == "PDFKIT_DEPRECATED_WITH_REPLACEMENT"
				|| text == "PDFKIT_UNAVAILABLE"
				|| text == "__API_AVAILABLE"
				|| text == "__API_UNAVAILABLE"
				|| text == "WEBKIT_AVAILABLE_MAC"
				|| text == "WEBKIT_DEPRECATED_MAC"
				|| text == "__OSX_AVAILABLE_BUT_DEPRECATED"
				|| text == "GK_API_DEPRECATED"
				|| text == "ITLIB_AVAILABLE"
				|| text == "SCN_GL_DEPRECATED_MAC"
				|| text == "CS_AVAILABLE"
			;
		}

		bool ignoreText(string text)
		{
			return text == "AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER"
				|| text == "AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER"
				|| text == "AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER"
				|| text == "AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER"
				|| text == "AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER"
				|| text == "AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER"
				|| text == "AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER"
				|| text == "AVAILABLE_MAC_OS_X_VERSION_10_7_AND_LATER"
				|| text == "AVAILABLE_MAC_OS_X_VERSION_10_8_AND_LATER"
				|| text == "AVAILABLE_MAC_OS_X_VERSION_10_9_AND_LATER"
				|| text == "AVAILABLE_MAC_OS_X_VERSION_10_10_AND_LATER"
				|| text == "AVAILABLE_MAC_OS_X_VERSION_10_11_AND_LATER"
				|| text == "AVAILABLE_MAC_OS_X_VERSION_10_12_AND_LATER"
				|| text == "AVAILABLE_MAC_OS_X_VERSION_10_13_AND_LATER"
				|| text == "AVAILABLE_MAC_OS_X_VERSION_10_14_AND_LATER"
				|| text == "AVAILABLE_MAC_OS_X_VERSION_10_15_AND_LATER"
				|| text == "NS_UNAVAILABLE"
				|| text == "CS_TVOS_UNAVAILABLE"
				|| text == "API_TO_BE_DEPRECATED"
				|| text == "NS_OPENGL_DEPRECATED"
				|| text == "DEPRECATED_ATTRIBUTE"
			;
		}

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
			if (!interfaces.Exists(x => x.name == token.Text && x.protocol))
				interfaces.Add(new Class(token.Text, true));
			Class new_item = interfaces.Find(x => x.name == token.Text && x.protocol);
			token = token.Next;

			while (token.Valid)
			{
				if (token.Text == "\n")
				{
					token = Details(token.Next, new_item);
					break;
				}
				else if (token.Text == ";")
				{
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
			if (!interfaces.Exists(x => x.name == token.Text && !x.protocol))
				interfaces.Add(new Class(token.Text, false));
			Class new_item = interfaces.Find(x => x.name == token.Text && !x.protocol);
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

			if (clazz is Class inter)
			{
				inter.methods.Add(new_item);
			}

			int mode = 0;
			string type = "";
			string new_part = "";
			int count = 0;

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
				else if (ignoreText(token.Text))
				{
				}
				else if (isCleanupTag(token.Text))
				{
					while (token.Valid)
					{
						if (token.Text == ";")
						{
							token = token.Prev;
							break;
						}
						else if (token.Text == "\n")
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
					count = 0;
					mode = 2;
				}
				else if (!token.isSymbol && mode == 2)
				{
					if (token.Text != "nullable" && token.Text != "_Nullable")
					{
						type += token.Text;
						if (!token.Next.isSymbol)
							type += " ";
					}
				}
				else if (token.Text == "(" && mode == 2)
				{
					type += token.Text;
					count++;
				}
				else if (token.Text == ")" && mode == 2)
				{
					count--;
					if (count < 0)
					{
						if (new_item.name == null)
						{
							new_item.returnType = type.Trim();
							mode = 0;
						}
						else
						{
							new_item.paramType.Add(type.Trim());
							mode = 3;
						}
						type = "";
					}
					else
					{
						type += token.Text;
					}
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

			if (clazz is Class inter)
			{
				inter.properties.Add(new_item);
			}

			int mode = 0;
			string type = "";
			string name = "";
			bool attributesDone = false;
			int count2 = 0;

			while (token.Valid)
			{
				if (token.Text == ";")
				{
					new_item.type = type;
					new_item.SetName(name);
					break;
				}
				else if (ignoreText(token.Text))
				{
				}
				else if (isCleanupTag(token.Text))
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
				else if (token.Text == "(" && !attributesDone)
				{
					mode = 1;
				}
				else if (!token.isSymbol && mode == 0)
				{
					if (token.Text != "_Nullable")
					{
						type += name;
						name = token.Text;
					}
				}
				else if ((token.Text == "^" || token.Text == "*") && count2 == 1 && mode == 0)
				{
					type = type + token.Text;
					name = token.Next.Text;
					mode = 3;
					token = token.Next;
				}
				else if (token.Text == "(" && mode == 0)
				{
					count2++;
					type += name + token.Text;
					name = "";
				}
				else if (token.Text == ")" && mode == 0)
				{
					count2--;
					type += name + token.Text;
					name = "";
				}
				else if (token.isSymbol && mode == 0 && token.Text != "\n")
				{
					type += name + token.Text;
					name = "";
				}
				else if (!token.isSymbol && mode == 3)
				{
					if (token.Text != "_Nullable")
					{
						type += token.Text;
					}
				}
				else if (token.isSymbol && mode == 3 && token.Text != "\n")
				{
					type += token.Text;
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
					attributesDone = true;
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
				if (name.Next.Next.Text == ";")
					return token;

				enums.Add(new Enum(token.Prev.Text == "NS_OPTIONS"));
				new_item = enums[enums.Count - 1];
				new_item.name = name.Text;
				new_item.type = type.Text;
				token = name.Next.Next.Next;
			}

			int mode = 0;

			while (token.Valid)
			{
				if (token.Text == "}")
				{
					while (new_item.values.Count < new_item.members.Count)
						new_item.values.Add("");

					for (int a = 0; a < new_item.values.Count; a++)
						new_item.values[a] = new_item.values[a].Trim();

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
				else if (isCleanupTag(token.Text) && mode == 1)
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
					new_item.values[new_item.values.Count - 1] += token.Text;
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

		public void AddFramework(string name)
		{
			if (!frameworks.Exists(x => x.name == name))
				frameworks.Add(new Framework(name));
			Framework newItem = frameworks.Find(x => x.name == name);
			interfaces = newItem.interfaces;
			enums = newItem.enums;
		}
	}
}
