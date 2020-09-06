using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;

namespace DaniSimController.Extensions
{
    public static class EnumExtensions
    {
        public static IReadOnlyCollection<T> GetEnums<T>()
            where T : Enum
            => Enum.GetValues(typeof(T)).Cast<T>().ToList();

        public static IReadOnlyCollection<T> GetEnums<T>(this T e)
            where T : Enum
            => Enum.GetValues(typeof(T)).Cast<T>().ToList();
    }
}
