
#pragma once

#include <string>
#include <memory>

#include "Export.h"
#include "Signal.h"
#include "Node.h"
#include "AttributeImpl.h"

namespace dbcppp
{
	class DBCPPP_API SignalImpl
		: public Signal
	{
	public:
		virtual const std::string& getName() const override;
		virtual Multiplexer getMultiplexerIndicator() const override;
		virtual uint64_t getMultiplexerSwitchValue() const override;
		virtual uint64_t getStartBit() const override;
		virtual uint64_t getBitSize() const override;
		virtual ByteOrder getByteOrder() const override;
		virtual ValueType getValueType() const override;
		virtual double getFactor() const override;
		virtual double getOffset() const override;
		virtual double getMinimum() const override;
		virtual double getMaximum() const override;
		virtual std::string getUnit() const override;
		virtual bool hasReceiver(const std::string& name) const override;
		virtual std::vector<const std::string*> getReceivers() const override;
		virtual const std::string* getValueDescriptionById(uint64_t id) const override;
		virtual std::vector<std::pair<uint64_t, const std::string*>> getValueDescriptions() const override;
		virtual const Attribute* getAttributeValueByName(const std::string& name) const override;
		virtual std::vector<std::pair<std::string, const Attribute*>> getAttributeValues() const override;
		virtual const std::string& getComment() const override;
		virtual boost::optional<Signal::ExtendedValueType> getExtendedValueType() const override;
		ErrorCode getError() const;

		SignalImpl() = default;
		SignalImpl(ByteOrder byte_order, ValueType value_type, uint64_t bit_size, uint64_t start_bit, uint64_t message_size = 8);

		std::string _name;
		Signal::Multiplexer _multiplexer_indicator;
		uint64_t _multiplexer_switch_value;
		uint64_t _start_bit;
		uint64_t _bit_size;
		Signal::ByteOrder _byte_order;
		Signal::ValueType _value_type;
		double _factor;
		double _offset;
		double _minimum;
		double _maximum;
		std::string _unit;
		std::set<std::string> _receivers;
		std::map<std::string, AttributeImpl> _attribute_values;
		std::map<uint64_t, std::string> _value_descriptions;
		std::string _comment;
		boost::optional<Signal::ExtendedValueType> _extended_value_type;
		// for performance
		uint64_t _mask;
		uint64_t _mask_signed;
		uint64_t _fixed_start_bit;
		uint64_t _fixed_start_bit_fd;
		uint64_t _byte_pos_fd;

		Signal::ErrorCode _error;
	};
}