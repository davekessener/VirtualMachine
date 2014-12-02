#ifndef JSON_TAG_H
#define JSON_TAG_H

#include <iosfwd>
#include <cstdint>

namespace json
{
	enum class json_id_t : std::uint8_t
	{
		OBJECT,
		ARRAY,
		STRING,
		NUMBER,
		BOOLEAN,
		NULL_JS
	};

	class Tag
	{
		public:
		typedef std::shared_ptr<Tag> Tag_ptr;
		typedef const std::string& name_t;

		public:
			void print(std::ostream&) const;
			json_id_t getID( ) const;
		private:
			virtual void i_doPrint(std::ostream&) const = 0;
			virtual json_id_t i_doGetID( ) const = 0;
	};

	typedef Tag::Tag_ptr Tag_ptr;

	class Array : public Tag, public std::vector<Tag_ptr>
	{
		public:
		using Tag::name_t;
		typedef std::shared_ptr<Array> Array_ptr;

		public:
			void addTag(Tag_ptr);
			inline void addObjectTag(Object_ptr p)
				{ addTag(std::dynamic_pointer_cast<Object>(p)); }
			inline void addArrayTag(Array_ptr p)
				{ addTag(std::dynamic_pointer_cast<Array>(p)); }
			inline void addStringTag(String_ptr p)
				{ addTag(std::dynamic_pointer_cast<String>(p)); }
			inline void addNumberTag(Number_ptr p)
				{ addTag(std::dynamic_pointer_cast<Number>(p)); }
			inline void addBooleanTag(Boolean_ptr p)
				{ addTag(std::dynamic_pointer_cast<Boolean>(p)); }
			inline void addNullTag(Null_ptr p)
				{ addTag(std::dynamic_pointer_cast<Null>(p)); }
		private:
			void i_doPrint(std::ostream&) const;
			json_id_t i_doGetID( ) const { return json_id_t::ARRAY; }
	};

	typedef Array::Array_ptr Array_ptr;

	class Object : public Tag
	{
		public:
		using Tag::name_t;
		typedef std::shared_ptr<Object> Object_ptr;

		public:
			void setTag(name_t, Tag_ptr);
			inline void setObjectTag(name_t s, Object_ptr p)
				{ set(s, std::dynamic_pointer_cast<Tag>(p); }
			inline void setArrayTag(name_t s, Array_ptr p)
				{ set(s, std::dynamic_pointer_cast<Tag>(p); }
			inline void setStringTag(name_t, String_ptr p)
				{ set(s, std::dynamic_pointer_cast<Tag>(p); }
			inline void setNumberTag(name_t, Number_ptr p)
				{ set(s, std::dynamic_pointer_cast<Tag>(p); }
			inline void setBooleanTag(name_t, Boolean_ptr p)
				{ set(s, std::dynamic_pointer_cast<Tag>(p); }
			inline void setNullTag(name_t, Null_ptr p)
				{ set(s, std::dynamic_pointer_cast<Tag>(p); }
			inline void setString(name_t s, const std::string& v)
				{ set(s, std::make_shared<String>(v)); }
			inline void setDouble(name_t s, double v)
				{ set(s, std::make_shared<Number>(v)); }
			inline void setInt(name_t s, int v)
				{ set(s, std::make_shared<Number>(v)); }
			inline void setBoolean(name_t s, bool v)
				{ set(s, std::make_shared<Boolean>(v)); }
			inline void setNull(name_t s)
				{ set(s, std::make_shared<Null>(v)); }
			Tag_ptr getTag(name_t);
			Object_ptr getObjectTag(name_t s)
				{ return std::dynamic_pointer_cast<Object>(getTag(s)); }
			Array_ptr getArrayTag(name_t s)
				{ return std::dynamic_pointer_cast<Array>(getTag(s)); }
			String_ptr getStringTag(name_t s)
				{ return std::dynamic_pointer_cast<String>(getTag(s)); }
			Number_ptr getNumberTag(name_t s)
				{ return std::dynamic_pointer_cast<Number>(getTag(s)); }
			Boolean_ptr getBooleanTag(name_t s)
				{ return std::dynamic_pointer_cast<Boolean>(getTag(s)); }
			Null_ptr getNullTag(name_t s)
				{ return std::dynamic_pointer_cast<Null>(getTag(s)); }
			std::string getString(name_t s) const
				{ return getStringTag(s)->toString(); }
			double getDouble(name_t s) const
				{ return getNumberTag(s)->toDouble(); }
			long getInt(name_t s) const
				{ return getNumberTag(s)->toInt(); }
			bool getBoolean(name_t s) const
				{ return getBooleanTag(s)->toBool(); }
		private:
			void i_doPrint(std::ostream&) const;
			json_id_t i_doGetID( ) const { return json_id_t::OBJECT; }
	};

	typedef Object::Object_ptr Object_ptr;
}

#endif

