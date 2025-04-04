#pragma once
#include <Base.h>
enum class NodeStatus { SUCCESS, FAIL, RUNNING };

class INode  abstract // �߻�
{
public:
	virtual NodeStatus Execute() PURE;
	virtual ~INode() = default;
};