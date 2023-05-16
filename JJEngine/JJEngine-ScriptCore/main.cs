using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace JJEngine_ScriptCore
{
    static class Graphics
    {
        public static void SetClearColor(float r, float g, float b)
        {
            Console.Write("SetClearColor");
            InternalCalls.Graphics_SetClearColor(r,g,b);
        }
    }

    public class main
    {
        public main()
        {
            Console.WriteLine("Script Core!!!!! 12314 ");
            test();
        }

        public void test()
        {
            Console.WriteLine("test!!!!!");
            Graphics.SetClearColor(1.0f, 0.5f, 0.3f);
        }
    }

    public class test
    {
        public string iamstr = "yes i am";
        public string test_str
        {
            get { return "test_str returned"; }
        }

        public void run(float val)
        {
            Console.WriteLine("test class run! {0}", val);
        }
    }


}
