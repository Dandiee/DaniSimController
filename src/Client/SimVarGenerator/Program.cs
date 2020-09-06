using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;

namespace SimVarGenerator
{
    class Program
    {
        static void Main(string[] args)
        {

            var rawVars = GetVars();
            var uniqueUnitNames = rawVars.Select(v => v.unit).Distinct(StringComparer.OrdinalIgnoreCase);
            var unitsDictionary = uniqueUnitNames.ToDictionary(kvp => kvp, kvp => new
            {
                Title = GetFriendlyName(kvp),
                Name = GetFriendlyName(kvp).Replace(" ", ""),
                OriginalName = kvp
            }, StringComparer.OrdinalIgnoreCase);

            var simVars = rawVars.Select(v =>
            {
                var title = GetFriendlyName(v.name);
                var simVarName = GetFriendlyName(v.name).Replace(" ", "");
                var type = unitsDictionary[v.unit];

                return
                    $"public static readonly SimVar {simVarName} = new SimVar(\"{v.name}\", \"{title}\", SimVarType.{type.Name});";
            });

            foreach (var simVar in simVars)
            {
                Console.WriteLine(simVar);
            }

            foreach (var unit in unitsDictionary)
            {
                Console.WriteLine(
                    $"public static readonly SimVarType {unit.Value.Name} = new SimVarType(\"{unit.Value.OriginalName}\", \"{unit.Value.Title}\");");
            }
        }

        private static IReadOnlyCollection<(string name, string unit, string settable)> GetVars()
            => File.ReadAllLines("SimVars.txt").Select(line =>
            {
                var data = line.Split('\t');

                return (data[0], data[1], data[2]);
            }).ToList();

        private static string GetFriendlyName(string unfriendlyName) =>
            CultureInfo.InvariantCulture.TextInfo.ToTitleCase(unfriendlyName
                .Replace('_', ' ')
                .Replace(':', ' ')
                .Replace('-', ' ')
                .Replace("/", "Per")
                .ToLowerInvariant());
    }
}
