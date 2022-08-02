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
        private const string WindowsApplicationDriverUrl = "http://127.0.0.1:4723/";
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
    }

    class Program
    {
        static void Main(string[] args)
        {
            DesktopSession desktopSession = new DesktopSession();
            System.Threading.Thread.Sleep(2000);

            bool bSuccess = false;

            try
            {
                //Paste generated code here
                Thread.Sleep(1000 * 10);

                // LeftClick on Window "" at (299,610)
                Console.WriteLine("LeftClick on Window \"\" at (299,610)");
                string xpath_LeftClickWindow_299_610 = "/Pane[@ClassName=\"#32769\"][@Name=\"デスクトップ 1\"]/Window[@ClassName=\"Window\"][@Name=\"Microsoft Visual Studio\"]/Window[@ClassName=\"Window\"]";
                var winElem_LeftClickWindow_299_610 = desktopSession.FindElementByAbsoluteXPath(xpath_LeftClickWindow_299_610);
                if (winElem_LeftClickWindow_299_610 != null)
                {
                    winElem_LeftClickWindow_299_610.Click();
                }
                else
                {
                    Console.WriteLine($"Failed to find element using xpath: {xpath_LeftClickWindow_299_610}");
                    return;
                }


                // LeftClick on Window "" at (401,683)
                Console.WriteLine("LeftClick on Window \"\" at (401,683)");
                string xpath_LeftClickWindow_401_683 = "/Pane[@ClassName=\"#32769\"][@Name=\"デスクトップ 1\"]/Window[@ClassName=\"Window\"][@Name=\"Microsoft Visual Studio\"]/Window[@ClassName=\"Window\"]";
                var winElem_LeftClickWindow_401_683 = desktopSession.FindElementByAbsoluteXPath(xpath_LeftClickWindow_401_683);
                if (winElem_LeftClickWindow_401_683 != null)
                {
                    winElem_LeftClickWindow_401_683.Click();
                }
                else
                {
                    Console.WriteLine($"Failed to find element using xpath: {xpath_LeftClickWindow_401_683}");
                    return;
                }

                Thread.Sleep(1000 * 30);


                // LeftClick on Button "新しいプロジェクトの作成" at (59,69)
                Console.WriteLine("LeftClick on Button \"新しいプロジェクトの作成\" at (59,69)");
                string xpath_LeftClickButton新しいプロジェクトの_59_69 = "/Pane[@ClassName=\"#32769\"][@Name=\"デスクトップ 1\"]/Window[@AutomationId=\"WorkflowHostView\"]/Custom[starts-with(@AutomationId,\"UserControl_\")]/Pane[starts-with(@AutomationId,\"ScrollViewer_\")]/Button[@Name=\"新しいプロジェクトの作成\"][starts-with(@AutomationId,\"Button_\")]";
                var winElem_LeftClickButton新しいプロジェクトの_59_69 = desktopSession.FindElementByAbsoluteXPath(xpath_LeftClickButton新しいプロジェクトの_59_69);
                if (winElem_LeftClickButton新しいプロジェクトの_59_69 != null)
                {
                    winElem_LeftClickButton新しいプロジェクトの_59_69.Click();
                }
                else
                {
                    Console.WriteLine($"Failed to find element using xpath: {xpath_LeftClickButton新しいプロジェクトの_59_69}");
                    return;
                }


                // LeftClick on Edit "テンプレートの検索(_S) (Alt+S)" at (188,22)
                Console.WriteLine("LeftClick on Edit \"テンプレートの検索(_S) (Alt+S)\" at (188,22)");
                string xpath_LeftClickEditテンプレートの検索__188_22 = "/Pane[@ClassName=\"#32769\"][@Name=\"デスクトップ 1\"]/Window[@AutomationId=\"WorkflowHostView\"]/Custom[@AutomationId=\"npdDialog\"]/Custom[starts-with(@AutomationId,\"UserControl_\")]/Group[@Name=\"Search Control\"][@AutomationId=\"SearchControl\"]/Edit[@Name=\"テンプレートの検索(_S) (Alt+S)\"][@AutomationId=\"PART_SearchBox\"]";
                var winElem_LeftClickEditテンプレートの検索__188_22 = desktopSession.FindElementByAbsoluteXPath(xpath_LeftClickEditテンプレートの検索__188_22);
                if (winElem_LeftClickEditテンプレートの検索__188_22 != null)
                {
                    winElem_LeftClickEditテンプレートの検索__188_22.Click();
                }
                else
                {
                    Console.WriteLine($"Failed to find element using xpath: {xpath_LeftClickEditテンプレートの検索__188_22}");
                    return;
                }


                // KeyboardInput VirtualKeys=""siv3d"" CapsLock=False NumLock=True ScrollLock=False
                Console.WriteLine("KeyboardInput VirtualKeys=\"\"siv3d\"\" CapsLock=False NumLock=True ScrollLock=False");
                System.Threading.Thread.Sleep(100);
                winElem_LeftClickEditテンプレートの検索__188_22.SendKeys("siv3d");


                // LeftClick on ListItem "OpenSiv3D(0.6.4)Web" at (336,34)
                Console.WriteLine("LeftClick on ListItem \"OpenSiv3D(0.6.4)Web\" at (336,34)");
                string xpath_LeftClickListItemOpenSiv3D0_336_34 = "/Pane[@ClassName=\"#32769\"][@Name=\"デスクトップ 1\"]/Window[@AutomationId=\"WorkflowHostView\"]/Custom[@AutomationId=\"npdDialog\"]/Custom[starts-with(@AutomationId,\"UserControl_\")]/List[@Name=\"プロジェクト テンプレート\"][@AutomationId=\"ListViewTemplates\"]/ListItem[@ClassName=\"ListBoxItem\"][@Name=\"OpenSiv3D(0.6.4)Web\"]";
                var winElem_LeftClickListItemOpenSiv3D0_336_34 = desktopSession.FindElementByAbsoluteXPath(xpath_LeftClickListItemOpenSiv3D0_336_34);
                if (winElem_LeftClickListItemOpenSiv3D0_336_34 != null)
                {
                    winElem_LeftClickListItemOpenSiv3D0_336_34.Click();
                }
                else
                {
                    Console.WriteLine($"Failed to find element using xpath: {xpath_LeftClickListItemOpenSiv3D0_336_34}");
                    return;
                }


                // LeftClick on Text "次へ(_N)" at (9,0)
                Console.WriteLine("LeftClick on Text \"次へ(_N)\" at (9,0)");
                string xpath_LeftClickText次へ_N_9_0 = "/Pane[@ClassName=\"#32769\"][@Name=\"デスクトップ 1\"]/Window[@AutomationId=\"WorkflowHostView\"]/Button[@Name=\"次へ(N)\"][@AutomationId=\"button_Next\"]/Text[@ClassName=\"TextBlock\"][@Name=\"次へ(_N)\"]";
                var winElem_LeftClickText次へ_N_9_0 = desktopSession.FindElementByAbsoluteXPath(xpath_LeftClickText次へ_N_9_0);
                if (winElem_LeftClickText次へ_N_9_0 != null)
                {
                    winElem_LeftClickText次へ_N_9_0.Click();
                }
                else
                {
                    Console.WriteLine($"Failed to find element using xpath: {xpath_LeftClickText次へ_N_9_0}");
                    return;
                }


                // LeftClick on Button "作成(C)" at (86,28)
                Console.WriteLine("LeftClick on Button \"作成(C)\" at (86,28)");
                string xpath_LeftClickButton作成C_86_28 = "/Pane[@ClassName=\"#32769\"][@Name=\"デスクトップ 1\"]/Window[@AutomationId=\"WorkflowHostView\"]/Button[@Name=\"作成(C)\"][@AutomationId=\"button_Next\"]";
                var winElem_LeftClickButton作成C_86_28 = desktopSession.FindElementByAbsoluteXPath(xpath_LeftClickButton作成C_86_28);
                if (winElem_LeftClickButton作成C_86_28 != null)
                {
                    winElem_LeftClickButton作成C_86_28.Click();
                }
                else
                {
                    Console.WriteLine($"Failed to find element using xpath: {xpath_LeftClickButton作成C_86_28}");
                    return;
                }

                Thread.Sleep(5000);


                // LeftClick on SplitButton "デバッグ ターゲット" at (117,20)
                Console.WriteLine("LeftClick on SplitButton \"デバッグ ターゲット\" at (117,20)");
                string xpath_LeftClickSplitButtonデバッグターゲット_117_20 = "/Pane[@ClassName=\"#32769\"][@Name=\"デスクトップ 1\"]/Window[@Name=\"OpenSiv3Dv0_6ForWeb1 - Microsoft Visual Studio\"][@AutomationId=\"VisualStudioMainWindow\"]/Pane[@Name=\"ToolBarDockTop\"]/ToolBar[@ClassName=\"ToolBar\"][@Name=\"標準\"]/SplitButton[@Name=\"デバッグ ターゲット\"][@AutomationId=\"PART_FocusTarget\"]";
                var winElem_LeftClickSplitButtonデバッグターゲット_117_20 = desktopSession.FindElementByAbsoluteXPath(xpath_LeftClickSplitButtonデバッグターゲット_117_20);
                if (winElem_LeftClickSplitButtonデバッグターゲット_117_20 != null)
                {
                    winElem_LeftClickSplitButtonデバッグターゲット_117_20.Click();
                }
                else
                {
                    Console.WriteLine($"Failed to find element using xpath: {xpath_LeftClickSplitButtonデバッグターゲット_117_20}");
                    return;
                }


                //test complete
                bSuccess = true;
            }
            finally
            {
                Assert.AreEqual(bSuccess, true);
            }
        }
    }
}
