#pragma once

#include "stdafx.h"
#include "replayengine.hpp"
#include "ui.hpp"
#include <array>
#include <memory>

template<size_t MAX_HISTORY>
class HistoryStack {
private:
	struct HistoryFrame {
		I64 start = 0;
		I64 end = 0;
		std::unique_ptr<replayengine::replayframe_t[]> frames = nullptr;
	};
	std::array<HistoryFrame, MAX_HISTORY> arr;
	size_t ptr = 0;
	size_t count = 0;
	void push(HistoryFrame &item) {
		arr[ptr] = std::move(item);
		ptr = (ptr + 1) % MAX_HISTORY;
		if (count < MAX_HISTORY)
			++count;
	}
	void pop() {
		if (count > 0) {
			ptr = (ptr - 1) % MAX_HISTORY;
			arr[ptr].frames = nullptr;
		}
	}
	const HistoryFrame &top() const {
		return arr[(ptr - 1) % MAX_HISTORY];
	}
	HistoryFrame &top() {
		return arr[(ptr - 1) % MAX_HISTORY];
	}
public:
	bool create_snapshot() {
		if (!ui::is_in_out_mark_consistent())
			return false;
		const I64 count = ui::mark_out - ui::mark_in + 1;
		if (count <= 0)
			return false;
		HistoryFrame hframe{
			ui::mark_in,
			ui::mark_out,
			std::make_unique<replayengine::replayframe_t[]>(count)
		};
		for (I64 i = 0; i < count; ++i) {
			hframe.frames[i] = replayengine::frames[ui::mark_in + i];
		}
		push(hframe);
		return true;
	}
	bool restore_snapshot() {
		const HistoryFrame &hframe = top();
		if (hframe.frames == nullptr)
			return false;
		const I64 count = hframe.end - hframe.start + 1;
		for (I64 i = 0; i < count; ++i) {
			replayengine::frames[hframe.start + i] = hframe.frames[i];
		}
		pop();
		return true;
	}
};