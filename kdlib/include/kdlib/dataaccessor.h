#pragma once

#include <vector>
#include <algorithm>

#include <boost/shared_ptr.hpp>

#include <kdlib/dbgtypedef.h>
#include <kdlib/variant.h>

namespace kdlib {

///////////////////////////////////////////////////////////////////////////////

class DataAccessor;
typedef boost::shared_ptr<DataAccessor> DataAccessorPtr;

class DataAccessor
{
public:

    virtual size_t getLength() const = 0;

    virtual unsigned char readByte(size_t pos=0) const = 0;
    virtual void writeByte(unsigned char value, size_t pos=0) = 0;

    virtual char readSignByte(size_t pos=0) const = 0;
    virtual void writeSignByte(char value, size_t pos=0) = 0;

    virtual unsigned short readWord(size_t pos=0) const = 0;
    virtual void writeWord(unsigned short value, size_t pos=0) = 0;

    virtual short readSignWord(size_t pos=0) const = 0;
    virtual void writeSignWord(short value, size_t pos=0) = 0;

    virtual unsigned long readDWord(size_t pos=0) const = 0;
    virtual void writeDWord(unsigned long value, size_t pos=0) = 0;

    virtual long readSignDWord(size_t pos=0) const = 0;
    virtual void writeSignDWord(long value, size_t pos=0) = 0;

    virtual unsigned long long readQWord(size_t pos=0) const = 0;
    virtual void writeQWord(unsigned long long value, size_t pos=0) = 0;

    virtual long long readSignQWord(size_t pos=0) const = 0;
    virtual void writeSignQWord(long long value, size_t pos=0) = 0;

    virtual float readFloat(size_t pos=0) const = 0;
    virtual void writeFloat(float value, size_t pos=0) = 0;

    virtual double readDouble(size_t pos=0) const = 0;
    virtual void writeDouble(double value, size_t pos=0) = 0;

    virtual void readBytes( std::vector<unsigned char>&  dataRange, size_t count, size_t pos=0) const = 0;
    virtual void writeBytes( const std::vector<unsigned char>&  dataRange, size_t pos=0) = 0;

    virtual void readWords( std::vector<unsigned short>&  dataRange, size_t count, size_t  pos=0) const = 0;
    virtual void writeWords( const std::vector<unsigned short>&  dataRange, size_t pos=0)  = 0;

    virtual void readDWords( std::vector<unsigned long>&  dataRange, size_t count, size_t  pos=0) const = 0;
    virtual void writeDWords( const std::vector<unsigned long>&  dataRange, size_t  pos=0)  = 0;

    virtual void readQWords( std::vector<unsigned long long>&  dataRange, size_t count, size_t  pos=0) const = 0;
    virtual void writeQWords( const std::vector<unsigned long long>&  dataRange, size_t  pos=0) = 0;

    virtual void readSignBytes( std::vector<char>&  dataRange, size_t count, size_t  pos=0) const = 0;
    virtual void writeSignBytes( const std::vector<char>&  dataRange, size_t  pos=0) = 0;

    virtual void readSignWords( std::vector<short>&  dataRange, size_t count, size_t  pos=0) const = 0;
    virtual void writeSignWords( const std::vector<short>&  dataRange, size_t  pos=0) = 0;

    virtual void readSignDWords( std::vector<long>&  dataRange, size_t count, size_t  pos=0) const = 0;
    virtual void writeSignDWords( const std::vector<long>&  dataRange, size_t  pos=0) = 0;

    virtual void readSignQWords( std::vector<long long>&  dataRange, size_t count, size_t  pos=0) const = 0;
    virtual void writeSignQWords( const std::vector<long long>&  dataRange, size_t  pos=0) = 0;

    virtual void readFloats( std::vector<float>&  dataRange, size_t count, size_t  pos=0) const = 0;
    virtual void writeFloats( const std::vector<float>&  dataRange, size_t  pos=0) = 0;

    virtual void readDoubles( std::vector<double>&  dataRange, size_t count, size_t  pos=0) const = 0;
    virtual void writeDoubles( const std::vector<double>&  dataRange, size_t  pos=0) = 0;

    virtual DataAccessorPtr nestedCopy( size_t  startOffset = 0, size_t  length = -1 ) = 0;
	virtual DataAccessorPtr externalCopy(MEMOFFSET_64  startAddr = 0, size_t  length = -1) = 0;

    virtual std::wstring getLocationAsStr() const = 0;

    virtual MEMOFFSET_64 getAddress() const = 0;
    virtual VarStorage getStorageType() const = 0;
    virtual std::wstring getRegisterName() const = 0;

	virtual bool checkRange(MEMOFFSET_64 startAddr, size_t length) const = 0;

	bool isRangeInside(MEMOFFSET_64 existRangeOffset, size_t existRangeLength, MEMOFFSET_64 newRangeOffset, size_t newRangeLength) const
	{
		MEMOFFSET_64 existRightBoundary = existRangeOffset + existRangeLength - 1;
		MEMOFFSET_64 newRightBoundary = newRangeOffset + newRangeLength - 1;
		if (std::clamp(newRangeOffset, existRangeOffset, existRightBoundary) != newRangeOffset ||
			std::clamp(newRightBoundary, existRangeOffset, existRightBoundary) != newRightBoundary) {
			return false;
		}
		else
		{
			return true;
		}
	}
};

///////////////////////////////////////////////////////////////////////////////

DataAccessorPtr getEmptyAccessor();
DataAccessorPtr getMemoryAccessor( MEMOFFSET_64  offset, size_t length);
DataAccessorPtr getRegisterAccessor(const std::wstring& registerName);

DataAccessorPtr getCacheAccessor(const std::vector<char>& buffer, const std::wstring&  location=L"");
DataAccessorPtr getCacheAccessor(size_t bufferSize, const std::wstring&  location=L"");
DataAccessorPtr getCacheAccessor(const void* rawBuffer, size_t bufferSize, const std::wstring&  location=L"");
DataAccessorPtr getCacheAccessor(const NumVariant& var, const std::wstring&  location=L"");

DataAccessorPtr getDumpAccessor(const std::vector<unsigned char>& buffer, MEMOFFSET_64 addr = 0, const std::wstring& location = L"");
DataAccessorPtr getDumpAccessor(size_t size, MEMOFFSET_64 addr = 0, const std::wstring& location = L"");
DataAccessorPtr getDumpAccessor(const NumVariant& var, MEMOFFSET_64 addr = 0, const std::wstring&  location = L"");

template<typename T, typename std::enable_if< !std::is_integral<T>::value >::type* = nullptr >
DataAccessorPtr getCacheAccessor(const T& structType, const std::wstring&  location=L"")
{
    return getCacheAccessor(&structType, sizeof(structType), location);
}

///////////////////////////////////////////////////////////////////////////////

class DataAccessorWrapper;
typedef boost::shared_ptr<DataAccessorWrapper> DataAccessorWrapperPtr;

class DataAccessorWrapper: public NumConvertable, private boost::noncopyable {
public:

	DataAccessorWrapper() {}
	~DataAccessorWrapper() {}

	DataAccessorWrapper (DataAccessorPtr ptr) : m_dataAccessor(ptr) {}

	DataAccessorPtr get() {
		return m_dataAccessor;
	}

	virtual kdlib::NumVariant getValue()  const {
		return kdlib::NumVariant(static_cast<const void*>(m_dataAccessor.get()));
	}

	DataAccessorWrapperPtr nestedCopy(size_t  startOffset = 0, size_t  length = -1) {
		return DataAccessorWrapperPtr(
			new DataAccessorWrapper(m_dataAccessor->nestedCopy(startOffset, length)));
	}

private:
	DataAccessorPtr m_dataAccessor;
};



///////////////////////////////////////////////////////////////////////////////

}
