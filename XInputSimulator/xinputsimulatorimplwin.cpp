
//    Copyright 2013 Dustin Bensing

//    This file is part of XInputSimulator.

//    XInputSimulator is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    any later version.

//    XInputSimulator is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser Public License for more details.

//    You should have received a copy of the GNU Lesser Public License
//    along with XInputSimulator.  If not, see <http://www.gnu.org/licenses/>.

#ifdef _WIN32

#include <Windows.h>
#include "xinputsimulatorimplwin.h"
#include "notimplementedexception.h"
#include "xinputsimulatorkeymapwin.h"
#include <iostream>



#define MOUSEEVENTF_HWHEEL 0x01000

XInputSimulatorImplWin::XInputSimulatorImplWin()
{
    this->initCurrentMousePosition();
}

void XInputSimulatorImplWin::initCurrentMousePosition()
{
    POINT p;
    if (GetCursorPos(&p))
    {
        this->currentX = p.x;
        this->currentY = p.y;
    }

    for (const auto & t:keys)
    {
        name_to_code_table.insert(name_to_code_table_t::value_type(std::get<0>(t), std::get<1>(t)));
    }
}

void XInputSimulatorImplWin::mouseMoveTo(int x, int y)
{
    SetCursorPos(x, y);

    this->currentX = x;
    this->currentY = y;
}

void XInputSimulatorImplWin::mouseMoveRelative(int x, int y)
{
    int newX = this->currentX + x;
    int newY = this->currentY + y;

    SetCursorPos(newX, newY);

    this->currentX = newX;
    this->currentY = newY;
}

//TODO use the button from parameter list
void XInputSimulatorImplWin::mouseDown(int button)
{
    INPUT in={0};
    in.type = INPUT_MOUSE;
    in.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1,&in,sizeof(INPUT));
    ZeroMemory(&in,sizeof(INPUT));
}

//TODO use the button from parameter list
void XInputSimulatorImplWin::mouseUp(int button)
{
    INPUT in={0};
    in.type = INPUT_MOUSE;
    in.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1,&in,sizeof(INPUT));
    ZeroMemory(&in,sizeof(INPUT));
}

//TODO use the button from parameter list
void XInputSimulatorImplWin::mouseClick(int button)
{
    this->mouseDown(button);
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    this->mouseUp(button);
}
//kajsdölfkjasdölfkjasldökfjaölsdkjfalsdkjfalskdjfaldskjfkajsdölfkjasdölfkjasldökfjaölsdkjfalsdkjfalskdjfaldskjfkajsdölfkjasdölfkjasldökfjaölsdkjfalsdkjfalskdjfaldskjfkajsdölfkjasdölfkjasldökfjaölsdkjfalsdkjfalskdjfaldskjfkajsdölfkjasdölfkjasldökfjaölsdkjfalsdkjfalskdjfaldskjfkajsdölfkjasdölfkjasldökfjaölsdkjfalsdkjfalskdjfaldskjfkajsdölfkjasdölfkjasldökfjaölsdkjfalsdkjfalskdjfaldskjfkajsdölfkjasdölfkjasldökfjaölsdkjfalsdkjfalskdjfaldskjfkajsdölfkjasdölfkjasldökfjaölsdkjfalsdkjfalskdjfaldskjfkajsdölfkjasdölfkjasldökfjaölsdkjfalsdkjfalskdjfaldskjfkajsdölfkjasdölfkjasldökfjaölsdkjfalsdkjfalskdjfaldskjfkajsdölfkjasdölfkjasldökfjaölsdkjfalsdkjfalskdjfaldskjfkajsdölfkjasdölfkjasldökfjaölsdkjfalsdkjfalskdjfaldskjfkajsdölfkjasdölfkjasldökfjaölsdkjfalsdkjfalskdjfaldskjfkajsdölfkjasdölfkjasldökfjaölsdkjfalsdkjfalskdjfaldskjfkajsdölfkjasdölfkjasldökfjaölsdkjfalsdkjfalskdjfaldskjfkajsdölfkjasdölfkjasldökfjaölsdkjfalsdkjfalskdjfaldskjf
void XInputSimulatorImplWin::mouseScrollX(int length)
{
    int scrollDirection = 1 * 50; // 1 left -1 right

    if(length < 0){
        length *= -1;
        scrollDirection *= -1;
    }

    for(int cnt = 0; cnt < length; cnt++)
    {
        INPUT in;
        in.type = INPUT_MOUSE;
        in.mi.dx = 0;
        in.mi.dy = 0;
        in.mi.dwFlags = MOUSEEVENTF_HWHEEL;
        in.mi.time = 0;
        in.mi.dwExtraInfo = 0;
        in.mi.mouseData = scrollDirection;// WHEEL_DELTA;
        SendInput(1,&in,sizeof(in));
    }
}

void XInputSimulatorImplWin::mouseScrollY(int length)
{
    int scrollDirection = -1 * 50; // 1 up -1 down

    if(length < 0){
        length *= -1;
        scrollDirection *= -1;
    }

    for(int cnt = 0; cnt < length; cnt++)
    {
        INPUT in;
        in.type = INPUT_MOUSE;
        in.mi.dx = 0;
        in.mi.dy = 0;
        in.mi.dwFlags = MOUSEEVENTF_WHEEL;
        in.mi.time = 0;
        in.mi.dwExtraInfo = 0;
        in.mi.mouseData = scrollDirection;// WHEEL_DELTA;
        SendInput(1,&in,sizeof(in));
    }
}

void XInputSimulatorImplWin::keyDown(int key)
{
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    input.ki.wScan = MapVirtualKey(input.ki.wVk, 0);
    input.ki.dwFlags = 0;

    input.ki.time = 0;
    input.ki.dwExtraInfo = ::GetMessageExtraInfo();
    SendInput(1, &input, sizeof(INPUT));
}

void XInputSimulatorImplWin::keyUp(int key)
{
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    input.ki.wScan = MapVirtualKey(input.ki.wVk, 0);
    input.ki.dwFlags = KEYEVENTF_KEYUP;

    input.ki.time = 0;
    input.ki.dwExtraInfo = ::GetMessageExtraInfo();
    SendInput(1, &input, sizeof(INPUT));
}

void XInputSimulatorImplWin::keyClick(int key)
{
    this->keyDown(key);
    this->keyUp(key);
}

int XInputSimulatorImplWin::charToKeyCode(char key_char)
{
//    throw NotImplementedException();
    return 0;
}

int XInputSimulatorImplWin::stringToKeyCode(std::string str)
{
    name_to_code_table_t::iterator it = name_to_code_table.find(str);
    if (it != name_to_code_table.end())
    {
        return it->second;
    }
    return 0;
}

void XInputSimulatorImplWin::keySequence(const std::string &sequence)
{
    std::cout << "key seq: " << sequence << std::endl;
    char t[2];
    t[1] = 0;
    for (const char c : sequence) {
        t[0] = tolower(c);
    //    std::cout << "cahr: " << c << std::endl;
        int keyCode = this->stringToKeyCode(t);
    //    std::cout << "key code: " << keyCode << std::endl;

        if (isupper(c)) {
        //    std::cout << "upper " << c << std::endl;

            this->keyDown(VK_LSHIFT);
            this->keyClick(keyCode);
            this->keyUp(VK_LSHIFT);
        }
        else {
            this->keyClick(keyCode);
        }


        std::cout << std::endl;
    }
}

#endif //win
