#pragma once
#include "DirectXCommon.h"
#include "Vector3.h"
#include <string>
#include <vector>

using namespace std;

enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
};

struct MapChipData {
	vector<vector<MapChipType>> data;
};

/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField {
public:
	void ResetMapChipdata();

	void LoadMapChipCsv(const string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVirtical();
	uint32_t GetNumBlockHorizontal();
	float GetkBlockWidth();
	float GetkBlockHeight();

private:
	MapChipData mapChipData_;

	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;

	static inline const uint32_t kNumBlockVirtical = 20;    // 縦幅
	static inline const uint32_t kNumBlockHorizontal = 100; // 横幅
};
