/// <summary>
/// Copyright (C) 2004-2025 Free Software Foundation, Inc.
///
/// Author: Alexander Gnauck AG-Software, mailto:gnauck@ag-software.de
///
/// This file is part of GNU Libidn.
///
/// GNU Libidn is free software: you can redistribute it and/or
/// modify it under the terms of either:
///
///   * the GNU Lesser General Public License as published by the Free
///     Software Foundation; either version 3 of the License, or (at
///     your option) any later version.
///
/// or
///
///   * the GNU General Public License as published by the Free
///     Software Foundation; either version 2 of the License, or (at
///     your option) any later version.
///
/// or both in parallel, as here.
///
/// GNU Libidn is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
/// General Public License for more details.
///
/// You should have received copies of the GNU General Public License and
/// the GNU Lesser General Public License along with this program.  If
/// not, see <https://www.gnu.org/licenses/>.
/// </summary>

using System;

namespace Gnu.Inet.Encoding
{
	
	public class PunycodeException : Exception
	{
		public static string OVERFLOW   = "Overflow.";
		public static string BAD_INPUT  = "Bad input.";
		
		/// <summary>
        /// Creates a new PunycodeException.
		/// </summary>
        /// <param name="message">message</param>
		public PunycodeException(string message) : base(message)
		{
		}
	}
}
