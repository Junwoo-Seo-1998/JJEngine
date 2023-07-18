using JJEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameScript.Source
{
    internal class EndScene : Entity
    {
        protected override void OnUpdate()
        {
            if (Input.IsPressed(KeyCode.F))
            {
                //just test
                SceneManager.LoadScene("./Resources/Scenes/Test.scn");
            }
        }
    };
}
