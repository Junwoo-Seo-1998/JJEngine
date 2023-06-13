using System;

namespace JJEngine_ScriptCore
{
    static class Graphics
    {
        public static void SetClearColor(float r, float g, float b)
        {
            Console.Write("SetClearColor");
            //InternalCalls.Graphics_SetClearColor(r,g,b);
        }
    }

    public class main
    {
        public main()
        {
            Console.WriteLine("yeay!");
            test();
        }

        public void test()
        {
            Console.WriteLine("color change");
            //Graphics.SetClearColor(1.0f, 0.8f, 0.3f);
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
