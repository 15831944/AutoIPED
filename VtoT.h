#ifndef VTOT_H              //��ֹ��ΰ���ͷ�ļ�
#define VTOT_H

//��_variant_t������ת����int
extern inline int  vtoi(_variant_t & v);
//��_variant_t������ת����CString
extern inline CString  vtos(_variant_t& v);
extern inline CString  vtos(COleVariant& v);
//��_variant_t������ת����double;
extern inline double  vtof(_variant_t &v);
//��_variant_t������ת����bool
extern inline bool  vtob( _variant_t &v);
extern inline bool  Btob( BOOL Bn);
extern inline bool  Dtob( double Dn);

#endif
