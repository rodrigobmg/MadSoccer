//------------------------------------------------------------------------
// Name:    Field.h
// Author:  jjuiddong
// Date:    2012-12-03
// 
// 축구장을 정의하는 클래스다.
//------------------------------------------------------------------------

#pragma once


namespace game
{
	class CField : public graphics::CWindow
	{
	public:
		CField();
		virtual ~CField();

	protected:

	public:
		virtual void	Move(int elapsTime) override;

	};

}
