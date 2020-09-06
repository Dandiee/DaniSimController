using System.Windows;
using System.Windows.Media;

namespace DaniSimController.Extensions
{
    public static class DependencyObjectExtensions
    {
        public static T GetParent<T>(this DependencyObject obj, object sender)
            where T : class
        {
            while (obj != null && obj != sender)
            {
                if (obj is T item)
                {
                    return item;
                }
                obj = VisualTreeHelper.GetParent(obj);
            }

            return null;
        }
    }
}
