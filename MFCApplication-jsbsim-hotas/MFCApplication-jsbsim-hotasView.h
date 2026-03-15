
// MFCApplication-jsbsim-hotasView.h: CMFCApplicationjsbsimhotasView 类的接口
//

#pragma once


class CMFCApplicationjsbsimhotasView : public CView
{
protected: // 仅从序列化创建
	CMFCApplicationjsbsimhotasView() noexcept;
	DECLARE_DYNCREATE(CMFCApplicationjsbsimhotasView)

// 特性
public:
	CMFCApplicationjsbsimhotasDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCApplicationjsbsimhotasView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MFCApplication-jsbsim-hotasView.cpp 中的调试版本
inline CMFCApplicationjsbsimhotasDoc* CMFCApplicationjsbsimhotasView::GetDocument() const
   { return reinterpret_cast<CMFCApplicationjsbsimhotasDoc*>(m_pDocument); }
#endif

