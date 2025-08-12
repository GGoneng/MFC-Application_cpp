// Lab02.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include "framework.h"
#include "Lab02.h"
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _CRT_SECURE_NO_WARNINGS

// 유일한 애플리케이션 개체입니다.

CWinApp theApp;

using namespace std;

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: 여기에 애플리케이션 동작을 코딩합니다.
            wprintf(L"심각한 오류: MFC 초기화 실패\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: 여기에 애플리케이션 동작을 코딩합니다.
            _tsetlocale(LC_ALL, _T(""));

            // 변수 생성
            char input[30];
            CString str;
            double d1;
            double d2;
            double result;
            int calc_idx;
            char calc;

            // 데이터 입력 받기
            printf("사칙연산 수식을 입력하시오:\n");
            fgets(input, sizeof(input), stdin);

            // Char arr -> CString 형변환
            str = input;

            // 기호 index 찾기
            calc_idx = str.FindOneOf(_T("+-*/"));

            // 기호와 입력 숫자 구분
            calc = str.GetAt(calc_idx);
            d1 = _wtof(str.Left(calc_idx - 1));
            d2 = _wtof(str.Right(str.GetLength() - calc_idx - 2));


            // 조건문을 통한 사칙연산 적용
            if (calc == '+') {
                result = d1 + d2;
                _tprintf(_T("%f"), result);
            }

            else if (calc == '-') {
                result = d1 - d2;
                _tprintf(_T("%f"), result);
            }

            else if (calc == '*') {
                result = d1 * d2;
                _tprintf(_T("%f"), result);
            }

            else if (calc == '/') {
                result = d1 / d2;
                _tprintf(_T("%f"), result);
            }

            else {
                _tprintf(_T("잘못된 기호 입력."));
            }
        }
    }
    else
    {
        // TODO: 오류 코드를 필요에 따라 수정합니다.
        wprintf(L"심각한 오류: GetModuleHandle 실패\n");
        nRetCode = 1;
    }

    return nRetCode;
}
