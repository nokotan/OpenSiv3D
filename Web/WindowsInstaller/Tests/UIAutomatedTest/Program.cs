//******************************************************************************
//
// Copyright (c) 2019 Microsoft Corporation. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//******************************************************************************

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using OpenQA.Selenium.Appium.Windows;
using OpenQA.Selenium.Remote;
using OpenQA.Selenium;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace UIXPathLib
{
    public class DesktopSession
    {
        private const string WindowsApplicationDriverUrl = "http://127.0.0.1:34723/";
        WindowsDriver<WindowsElement> desktopSession;

        public DesktopSession()
        {
            DesiredCapabilities appCapabilities = new DesiredCapabilities();
            appCapabilities.SetCapability("app", "Root");
            appCapabilities.SetCapability("deviceName", "WindowsPC");
            desktopSession = new WindowsDriver<WindowsElement>(new Uri(WindowsApplicationDriverUrl), appCapabilities);
        }

        ~DesktopSession()
        {
            desktopSession.Quit();
        }

        public WindowsDriver<WindowsElement> DesktopSessionElement
        {
            get { return desktopSession; }
        }

        public WindowsElement FindElementByAbsoluteXPath(string xPath, int nTryCount = 10)
        {
            WindowsElement uiTarget = null;

            while (nTryCount-- > 0)
            {
                try
                {
                    uiTarget = desktopSession.FindElementByXPath(xPath);
                }
                catch
                {
                }

                if (uiTarget != null)
                {
                    break;
                }
                else
                {
                    System.Threading.Thread.Sleep(2000);
                }
            }

            return uiTarget;
        }

        public void TakeScreenShot(string fileName)
        {
            var screenShot = desktopSession.GetScreenshot();
            screenShot.SaveAsFile(fileName, ScreenshotImageFormat.Png);
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            System.Threading.Thread.Sleep(10 * 1000);
            DesktopSession desktopSession = new DesktopSession();
            bool bSuccess = false;

            try
            {
                // Wait for Visual Studio Launched
                var visualStudioWindowPath = "/Pane[@ClassName=\"#32769\"][@Name=\"Desktop 1\"]/Window[@Name=\"Microsoft Visual Studio\"]/Custom/Pane/Button[@Name=\"Create a new project\"]";
                Console.WriteLine("Waiting for Visual Studio being ready...");
                var visualStudioWindow = desktopSession.FindElementByAbsoluteXPath(visualStudioWindowPath, 10 * 60 / 2);
                if (visualStudioWindow != null)
                {
                    Console.WriteLine("Ok");
                }
                else
                {
                    Console.WriteLine("Timeout!");
                }

                // LeftClick on Button "Create a new project" at (53,72)
                Console.WriteLine("LeftClick on Button \"Create a new project\" at (53,72)");
                string xpath_LeftClickButtonCreateanew_53_72 = "/Pane[@ClassName=\"#32769\"][@Name=\"Desktop 1\"]/Window[@Name=\"Microsoft Visual Studio\"]/Custom/Pane/Button[@Name=\"Create a new project\"]";
                var winElem_LeftClickButtonCreateanew_53_72 = desktopSession.FindElementByAbsoluteXPath(xpath_LeftClickButtonCreateanew_53_72);
                desktopSession.TakeScreenShot("Create a new project.png");
                if (winElem_LeftClickButtonCreateanew_53_72 != null)
                {
                    winElem_LeftClickButtonCreateanew_53_72.Click();
                }
                else
                {
                    Console.WriteLine($"Failed to find element using xpath: {xpath_LeftClickButtonCreateanew_53_72}");
                    return;
                }


                // LeftClick on Edit "_Search for templates (Alt+S)" at (115,10)
                Console.WriteLine("LeftClick on Edit \"_Search for templates (Alt+S)\" at (115,10)");
                string xpath_LeftClickEdit_Searchfor_115_10 = "/Pane[@ClassName=\"#32769\"][@Name=\"Desktop 1\"]/Window[@Name=\"Microsoft Visual Studio\"]/Custom/Custom/Group[@Name=\"Search Control\"]/Edit[@Name=\"_Search for templates (Alt+S)\"]";
                var winElem_LeftClickEdit_Searchfor_115_10 = desktopSession.FindElementByAbsoluteXPath(xpath_LeftClickEdit_Searchfor_115_10);
                desktopSession.TakeScreenShot("Search for templates.png");
                if (winElem_LeftClickEdit_Searchfor_115_10 != null)
                {
                    winElem_LeftClickEdit_Searchfor_115_10.Click();
                }
                else
                {
                    Console.WriteLine($"Failed to find element using xpath: {xpath_LeftClickEdit_Searchfor_115_10}");
                    return;
                }


                // KeyboardInput VirtualKeys=""siv3d"" CapsLock=False NumLock=True ScrollLock=False
                Console.WriteLine("KeyboardInput VirtualKeys=\"\"siv3d\"\" CapsLock=False NumLock=True ScrollLock=False");
                System.Threading.Thread.Sleep(100);
                winElem_LeftClickEdit_Searchfor_115_10.SendKeys("siv3d");


                // LeftClick on ListItem "OpenSiv3D(0.6.4)Web" at (450,89)
                Console.WriteLine("LeftClick on ListItem \"OpenSiv3D(0.6.4)Web\" at (450,89)");
                string xpath_LeftClickListItemOpenSiv3D0_450_89 = "/Pane[@ClassName=\"#32769\"][@Name=\"Desktop 1\"]/Window[@Name=\"Microsoft Visual Studio\"]/Custom/Custom/List[@Name=\"Project Templates\"]/ListItem[@ClassName=\"ListBoxItem\"][@Name=\"OpenSiv3D(0.6.4)Web\"]";
                var winElem_LeftClickListItemOpenSiv3D0_450_89 = desktopSession.FindElementByAbsoluteXPath(xpath_LeftClickListItemOpenSiv3D0_450_89);
                desktopSession.TakeScreenShot("OpenSiv3D(0.6.4)Web.png");
                if (winElem_LeftClickListItemOpenSiv3D0_450_89 != null)
                {
                    winElem_LeftClickListItemOpenSiv3D0_450_89.Click();
                }
                else
                {
                    Console.WriteLine($"Failed to find element using xpath: {xpath_LeftClickListItemOpenSiv3D0_450_89}");
                    return;
                }


                // LeftClick on Button "Next" at (85,26)
                Console.WriteLine("LeftClick on Button \"Next\" at (85,26)");
                string xpath_LeftClickButtonNext_85_26 = "/Pane[@ClassName=\"#32769\"][@Name=\"Desktop 1\"]/Window[@Name=\"Microsoft Visual Studio\"]/Button[@Name=\"Next\"]";
                var winElem_LeftClickButtonNext_85_26 = desktopSession.FindElementByAbsoluteXPath(xpath_LeftClickButtonNext_85_26);
                desktopSession.TakeScreenShot("Next.png");
                if (winElem_LeftClickButtonNext_85_26 != null)
                {
                    winElem_LeftClickButtonNext_85_26.Click();
                }
                else
                {
                    Console.WriteLine($"Failed to find element using xpath: {xpath_LeftClickButtonNext_85_26}");
                    return;
                }


                // LeftClick on Button "Create" at (103,27)
                Console.WriteLine("LeftClick on Button \"Create\" at (103,27)");
                string xpath_LeftClickButtonCreate_103_27 = "/Pane[@ClassName=\"#32769\"][@Name=\"Desktop 1\"]/Window[@Name=\"Microsoft Visual Studio\"]/Button[@Name=\"Create\"]";
                var winElem_LeftClickButtonCreate_103_27 = desktopSession.FindElementByAbsoluteXPath(xpath_LeftClickButtonCreate_103_27);
                desktopSession.TakeScreenShot("Create.png");
                if (winElem_LeftClickButtonCreate_103_27 != null)
                {
                    winElem_LeftClickButtonCreate_103_27.Click();
                }
                else
                {
                    Console.WriteLine($"Failed to find element using xpath: {xpath_LeftClickButtonCreate_103_27}");
                    return;
                }


                // LeftClick on SplitButton "Debug Target" at (107,17)
                Console.WriteLine("LeftClick on SplitButton \"Debug Target\" at (107,17)");
                string xpath_LeftClickSplitButtonDebugTarge_107_17 = "/Pane[@ClassName=\"#32769\"][@Name=\"Desktop 1\"]/Window[@Name=\"OpenSiv3Dv0_6ForWeb1 - Microsoft Visual Studio\"]/Pane[@Name=\"ToolBarDockTop\"]/ToolBar[@ClassName=\"ToolBar\"][@Name=\"Standard\"]/SplitButton[@Name=\"Debug Target\"]";
                var winElem_LeftClickSplitButtonDebugTarge_107_17 = desktopSession.FindElementByAbsoluteXPath(xpath_LeftClickSplitButtonDebugTarge_107_17);
                desktopSession.TakeScreenShot("Debug Target.png");
                if (winElem_LeftClickSplitButtonDebugTarge_107_17 != null)
                {
                    winElem_LeftClickSplitButtonDebugTarge_107_17.Click();
                }
                else
                {
                    Console.WriteLine($"Failed to find element using xpath: {xpath_LeftClickSplitButtonDebugTarge_107_17}");
                    return;
                }





                //test complete
                bSuccess = true;
            }
            finally
            {
                // Assert.AreEqual(bSuccess, true);
            }
        }
    }
}
