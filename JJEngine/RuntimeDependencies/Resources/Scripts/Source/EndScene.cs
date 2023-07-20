using JJEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Game
{
    class EndScene : Entity
    {
        protected override void OnUpdate()
        {
            if (Input.IsTriggered(KeyCode.F))
            {
                //just test
                SceneManager.LoadScene("./Resources/Scenes/Test.scn");
            }
        }
    };
}
