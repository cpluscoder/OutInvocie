#pragma once

/**
 Service�����洴������SystemȨ�޵Ĵ���Ӧ�ó���(ò�ƿ�����UAC)

 ����������û��������ͨ�ŵĽ���취
 1 ���������Ҫͨ��������Ϣ�ķ�ʽ���û�������ʹ��WTSSendMessage����
     �ú���������MessageBox��ͬ, ���ڲ���Ҫ����������UI�ķ�������һ�ּ����еķ���, �����㹻��ȫ, ��Ϊ����ʾ����Ϣ���޷����ڶ�ȡ�Եײ����Ŀ���
 2 ���������Ҫ��������UI, ��ʹ��CreateProcessAsUser�����ڷ���������û����洴������
     ��Ҫע��, ��Ȼ��Ҫ���´����Ľ��̺�ԭʼ����֮�����ͨѶ
 3 �����Ҫ˫�򽻻�, ʹ��Windows Communication Foundation(WCF)
     һ������.NET Remoting�������߻��κ��������̼�ͨѶ(IPC)�Ļ���(Windows��Ϣ����), ������ʵ�ֿ�Ự��ͨѶ��
	 �����Ҫ����, WCF��Remoting�Ǳ�ֱ�������û�(����UACû�й��ر�)����ȫ��������
*/

class CStartAppWithWnd
{
public:
	explicit CStartAppWithWnd(void);
	virtual ~CStartAppWithWnd(void);

	DWORD _stdcall LaunchWin7SessionProcess(LPTSTR lpCommand);
};

