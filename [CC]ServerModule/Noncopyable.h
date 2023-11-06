//////////////////////////////////////////////////////////////////////////////
/// \file Noncopyable.h
/// \author excel96
/// \date 2003.7.23
//////////////////////////////////////////////////////////////////////////////

#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

//////////////////////////////////////////////////////////////////////////////
/// \class Noncopyable
/// \brief ���� �����ڳ� ���� �����ڰ� ���Ǹ� ����� Ŭ�������� �� Ŭ������ 
/// private���� ����ϸ�, ������ �� ������ �߻��ϸ鼭 ���� �����ڳ� ���� 
/// �����ڸ� ����� �� ���ٰ� �˷��ش�.
//////////////////////////////////////////////////////////////////////////////

class Noncopyable
{
protected:
	Noncopyable() {}
	~Noncopyable() {}

private:
	Noncopyable(const Noncopyable&);
	const Noncopyable& operator = (const Noncopyable&);
};

#ifdef _MSC_VER
// ���� �����ڸ� �������� ���߽��ϴ�.
#pragma warning(disable:4511)

// �����Ϸ����� �־��� Ŭ������ ���� ���� �����ڸ� ������ �� �����ϴ�.
#pragma warning(disable:4512)
#endif

/// ������ Noncopyable�� �ٷ� ��ӹ��� ���ϴ� Ŭ��������, 
/// ���� ������ �� �����ڸ� ���� ���� ��ũ��
#define DECLARE_NONCOPYABLE(NAME) private: \
	NAME(const NAME&) {} \
	NAME& operator = (const NAME&) { return *this; }

#endif //__NONCOPYABLE_H__