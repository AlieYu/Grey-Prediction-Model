/**
*Author  : yzl
*Date    : 2015-4-28
*FileName: GM.cpp
*Function: 实现灰色GM(1,1)预测模型
*/
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
using namespace std;


//预定义元素类型
typedef double ElemType;


/**********************************************************************************

								    矩阵类

**********************************************************************************/
class Matrix{
	//输出矩阵
	friend ostream& operator <<(ostream &os,const Matrix &A);
public:
	//无参构造函数
	Matrix();
	//有参构造函数
	Matrix(int rows,int columns);
	//析构函数
	~Matrix();
	//复制构造函数
	Matrix(const Matrix& B);

	//创建矩阵
	void createMatrix(ElemType **arr);

	//重载矩阵赋值运算操作符
	Matrix& operator =(Matrix &B);
	//重载矩阵加法运算操作符
	Matrix operator +(Matrix &B);
	//重载矩阵乘法运算操作符
	Matrix operator *(Matrix &B);

	//矩阵转置
	Matrix& trans();
	//矩阵求逆
	Matrix& inverse();

	//取得矩阵元素
	ElemType** getMatrixElems();

public:
	int m_rows;          //矩阵行数
	int m_columns;       //矩阵列数
	ElemType **m_elems;  //矩阵元素
};

//无参构造函数
Matrix::Matrix(){
	m_rows=0;
	m_columns=0;
	m_elems=NULL;
}

//有参构造函数
Matrix::Matrix(int rows, int columns){
	m_rows=rows;
	m_columns=columns;
	m_elems=new ElemType*[rows];
	for(int i=0;i<rows;i++){
		m_elems[i]=new ElemType[columns];
	}
}

//析构函数
Matrix::~Matrix(){
	for(int i=0;i<m_rows;i++){
		delete[] m_elems[i];
	}
	delete[] m_elems;
}

//创建矩阵
void Matrix::createMatrix(ElemType **arr){
	for(int i=0;i<m_rows;i++){
		for(int j=0;j<m_columns;j++){
			m_elems[i][j]=arr[i][j];
		}
	}
}

//流输出
ostream& operator <<(ostream &os,const Matrix &A){
	for(int i=0;i<A.m_rows;i++){
		for(int j=0;j<A.m_columns;j++){
			cout<<A.m_elems[i][j]<<" ";
		}
		cout<<endl;
	}
	return os;
}

//矩阵复制构造函数
Matrix::Matrix(const Matrix &B){
	m_rows=B.m_rows;
	m_columns=B.m_columns;
	m_elems=new ElemType*[m_rows];
	int i=0;
	for(i=0;i<m_rows;i++){
		m_elems[i]=new ElemType[m_columns];
	}
	//复制
	for(i=0;i<m_rows;i++){
		for(int j=0;j<m_columns;j++){
			m_elems[i][j]=B.m_elems[i][j];
		}
	}
}


//矩阵赋值
Matrix& Matrix::operator =(Matrix &B){
	m_rows=B.m_rows;
	m_columns=B.m_columns;
	m_elems=new ElemType*[m_rows];
	int i=0;
	for(i=0;i<m_rows;i++){
		m_elems[i]=new ElemType[m_columns];
	}
	//赋值
	for(i=0;i<m_rows;i++){
		for(int j=0;j<m_columns;j++){
			m_elems[i][j]=B.m_elems[i][j];
		}
	}
	return *this;
}


//矩阵相加
Matrix Matrix::operator +(Matrix &B){
	if(B.m_rows!=m_rows || B.m_columns!=m_columns){
		throw invalid_argument("The Matrix should be matched");
	}
	Matrix &T=*new Matrix;
	T.m_rows=m_rows;
	T.m_columns=m_columns;
	T.m_elems=new ElemType*[m_rows];
	int i=0;
	for(i=0;i<m_rows;i++){
		T.m_elems[i]=new ElemType[m_columns];
	}
	//相加
	for(i=0;i<m_rows;i++){
		for(int j=0;j<m_columns;j++){
			T.m_elems[i][j]=m_elems[i][j]+B.m_elems[i][j];
		}
	}
	return T;
}

//矩阵乘法
Matrix Matrix::operator *(Matrix &B){
	if(m_columns!=B.m_rows){
		throw invalid_argument("The Matrix should be matched");
	}
	Matrix &T=*new Matrix;
	T.m_rows=m_rows;
	T.m_columns=B.m_columns;
	T.m_elems=new ElemType*[T.m_rows];
	int i=0;
	for(i=0;i<T.m_rows;i++){
		T.m_elems[i]=new ElemType[T.m_columns];
	}
	//矩阵相乘
	for(i=0;i<T.m_rows;i++){
		for(int j=0;j<T.m_columns;j++){
			T.m_elems[i][j]=0;
			for(int k=0;k<m_columns;k++){
				T.m_elems[i][j]+=m_elems[i][k]*B.m_elems[k][j];
			}
		}
	}
	return T;
}


//矩阵转置
Matrix& Matrix::trans(){
	Matrix &T=*new Matrix;
	T.m_rows=m_columns;
	T.m_columns=m_rows;
	T.m_elems=new ElemType*[T.m_rows];
	int i=0;
	for(i=0;i<T.m_rows;i++){
		T.m_elems[i]=new ElemType[T.m_columns];
	}
	//转置
	for(i=0;i<T.m_rows;i++){
		for(int j=0;j<T.m_columns;j++){
			T.m_elems[i][j]=m_elems[j][i];
		}
	}
	return T;
}


//矩阵求逆
//@param arr是待求逆的矩阵,因为针对灰色模型,B的转置乘以B一定是二阶方阵
//所以,这里的求逆方法是针对二阶方阵的
Matrix& Matrix::inverse(){
	//先判断该二阶方阵是否可逆
	ElemType temp=(m_elems[0][0]*m_elems[1][1]-m_elems[0][1]*m_elems[1][0]);
	if(temp==0 ){
		cout<<"The Matrix can not be inversed"<<endl;
		exit(1);
	}
	Matrix &T=*new Matrix;
	T.m_rows=2;
	T.m_columns=2;
	T.m_elems=new ElemType*[T.m_rows];
	for(int i=0;i<T.m_rows;i++){
		T.m_elems[i]=new ElemType[T.m_columns] ;
	}
	T.m_elems[0][0]=(ElemType)m_elems[1][1]/temp;
	T.m_elems[0][1]=(ElemType)-m_elems[0][1]/temp;
	T.m_elems[1][0]=(ElemType)-m_elems[1][0]/temp;
	T.m_elems[1][1]=(ElemType)m_elems[0][0]/temp;
	return T;
}

//取得矩阵元素
ElemType** Matrix::getMatrixElems(){
	return m_elems;
}





/**********************************************************************************

								   灰色模型基类

**********************************************************************************/
class GrayModel{
public:
	//构造函数
	//@param data 待预测的原始序列
	//@param num  待预测的原始序列的长度
	GrayModel(ElemType *data,int num);

	//析构函数
	~GrayModel();
	
	//对原始序列进行一次累加生成(AGO)
	vector<ElemType> srcdata_AGO();
	
	//对一次累加生成序列计算紧邻均值序列(灰色模型背景值)
	//@param ratio 紧邻均值序列生成的系数,传统取值为0.5
	vector<ElemType> srcdata_mean(double ratio);
	
	//设置B矩阵
	virtual ElemType** setMatrix_B(vector<ElemType> meanSquence);
	
	//利用最小二乘估计估算灰色模型参数
	void calculate(double &a,double &b,vector<ElemType> meanSquence);
	
private:
	//待预测的原始序列
	vector<ElemType> srcData;
};



GrayModel::GrayModel(ElemType *data,int num){
	for(int i=0;i<num;i++){
		srcData.push_back(data[i]);
	}
}

GrayModel::~GrayModel(){
}

//生成一次累加序列
vector<ElemType> GrayModel::srcdata_AGO(){
	vector<ElemType> data_AGO;
	//数据序列长度
	int dataLen=srcData.size(); 
	//进行一次累加
	ElemType temp=0;
	for(int i=0;i<dataLen;i++){
		temp+=srcData[i];
		data_AGO.push_back(temp);
	}
	return data_AGO;
}

//对一次累加生成序列计算紧邻均值序列(灰色模型背景值)
	//@param ratio 紧邻均值序列生成的系数,传统取值为0.5  (在实际应用中，应根据实际的数据序列来优选背景值，
//可用比如粒子群优化算法，将其适应度函数选为使得拟合序列与原始序列的相对误差的平方均值的倒数)
vector<ElemType> GrayModel::srcdata_mean(double ratio){
	//先生成一次累加序列
	vector<ElemType> res=srcdata_AGO();
	//紧邻均值序列
	vector<ElemType> meanSquence;
	//计算均值序列
	int len=res.size();
	for(int i=0;i<(len-1);i++) {
		meanSquence.push_back ( (res[i]+res[i+1])*ratio );
	}
	return meanSquence;
	
	
	//中间变量    优选的背景值
	//ElemType temp=0,temp1=0,temp2=0;
	
	/*int cnt=0;   
	for(int i=0,len=res.size();i<len;){
		cnt++;
		temp1=ratio*res[i];
		temp2=(1-ratio)*res[++i];
		temp=temp1+temp2;
		meanSquence.push_back(temp);
		if(cnt==(len-1)) break;
	}*/

	//return meanSquence;
}
//设置B矩阵
ElemType** GrayModel::setMatrix_B(vector<ElemType> meanSquence){
	return 0;
}

//利用最小二乘估计估算灰色模型参数<a,b>
void GrayModel::calculate(double &a, double &b,vector<ElemType> meanSquence){
	//先生成B矩阵数组
	int dataLen=srcData.size();
	ElemType **B_arr=new ElemType*[dataLen-1]; //行数
	int i=0;
	for(i=0;i<(dataLen-1);i++){
		B_arr[i]=new ElemType[2];              //列数
	}
	B_arr=setMatrix_B(meanSquence);
	
	//生成B矩阵
	Matrix B(dataLen-1,2);
	B.createMatrix(B_arr);
	
	//计算B的转置
	Matrix B_trans=B.trans();
	
	//计算B的转置与B的乘积
	Matrix C=B_trans*B;
	
	//计算矩阵C的逆
	Matrix C_inv=C.inverse();
	
	//计算矩阵C_inv与B_trans的乘积
	Matrix D=C_inv*B_trans;
	
	//生成Y矩阵
	ElemType **Y_arr=new ElemType*[dataLen-1];

	for(i=0;i<(dataLen-1);i++){
		Y_arr[i]=new ElemType[1];
	}
	for(i=0;i<(dataLen-1);i++){
		Y_arr[i][0]=srcData[i+1];
	}
	Matrix Y(dataLen-1,1);
	Y.createMatrix(Y_arr);
	
	//计算D矩阵与Y矩阵的乘积
	Matrix res=D*Y;
	
	//得到灰色模型方程的系数
	a=res.getMatrixElems()[0][0];
	b=res.getMatrixElems()[1][0];
	
	
	//销毁内存
	for(i=0;i<(dataLen-1);i++){
		delete[] B_arr[i];
	}
	delete [] B_arr;
	for(i=0;i<(dataLen-1);i++){
		delete[] Y_arr[i];
	}
	delete [] Y_arr;
}





/**********************************************************************************

								    GM(1,1)模型子类

**********************************************************************************/
class GM: public GrayModel{
public:
	GM(ElemType *data,int num):GrayModel(data,num){}
	~GM(){}
	ElemType** setMatrix_B(vector<ElemType> meanSquence);
	//得到原始序列的预测值
	vector<ElemType> getSrcDataPrediction(double a,double b,ElemType initValue,int dataLen);
};

ElemType** GM::setMatrix_B(vector<ElemType> meanSquence){
	//根据均值序列的大小动态创建数组
	int meanSize=meanSquence.size();
	ElemType **B=new ElemType*[meanSize]; //行数
	int i=0;
	for(i=0;i<meanSize;i++){
		B[i]=new ElemType[2];             //列数
	}
	//生成B矩阵
	for(i=0;i<meanSize;i++){
		B[i][0]=-meanSquence[i];
		B[i][1]=1;
	}
	//返回
	return B;
}

vector<ElemType> GM::getSrcDataPrediction(double a,double b,ElemType initValue,int dataLen){
	//先计算累加序列预测模型值
	vector<ElemType> prediction;
	prediction.push_back(initValue);
	int i=0;
	ElemType temp=0.0;
	for(i=1;i<dataLen;i++){
		temp=(initValue-b/a)*exp(-a*i)+(b/a);
		prediction.push_back(temp);
	}
	//计算原始序列预测值
	vector<ElemType> res;
	res.push_back(initValue);
	for(i=1;i<dataLen;i++){
		temp=prediction[i]-prediction[i-1];
		res.push_back(temp);
	}
	return res;
}






