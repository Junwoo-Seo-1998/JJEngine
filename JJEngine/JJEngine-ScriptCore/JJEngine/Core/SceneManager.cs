using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JJEngine
{
    public class SceneManager
    {
        public static void LoadScene(string sceneName)
        {
            InternalCalls.SceneManager_LoadScene(sceneName);
        }
    }
}
