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

struct IndexSet {
	uint32_t xIndex;
	uint32_t yIndex;
};

struct BlockRect {
	float left;
	float right;
	float bottom;
	float top;
};

/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField {
public:
	void ResetMapChipdata();

	void LoadMapChipCsv(const string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);//インデックスから座標を取得

	uint32_t GetNumBlockVirtical();
	uint32_t GetNumBlockHorizontal();
	float GetkBlockWidth();
	float GetkBlockHeight();
	//指定座標がマップチップの何番の位置にあるか
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);
	BlockRect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

private:
	MapChipData mapChipData_;

	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;

	static inline const uint32_t kNumBlockVirtical = 20;    // 縦幅
	static inline const uint32_t kNumBlockHorizontal = 150; // 横幅
};
