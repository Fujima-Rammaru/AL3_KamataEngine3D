#include "MapChipField.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

namespace {

map<string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};

}

// マップチップデータをリセット
void MapChipField::ResetMapChipdata() {
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const string& filePath) {

	ResetMapChipdata();

	// ファイルを開く
	ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// mapChipCSV
	stringstream mapChipCsv;
	// ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();
	// ファイルを閉じる
	file.close();

	// CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		string line;
		getline(mapChipCsv, line);

		// 1行分の文字列をストリームに変換して解析しやすくなる
		istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {

	// 範囲外を指定されたら空白チップを返す
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex];
}

// 縦横のインデックスを指定してその位置のマップチップのワールド座標を取得する
Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0); }

uint32_t MapChipField::GetNumBlockVirtical() { return kNumBlockVirtical; }

uint32_t MapChipField::GetNumBlockHorizontal() { return kNumBlockHorizontal; }

float MapChipField::GetkBlockHeight() { return kBlockHeight; }

// 座標からマップチップ番号を計算
IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {
	IndexSet indexSet{};
	indexSet.xIndex = uint32_t((position.x + kBlockWidth / 2) / kBlockWidth);
	uint32_t beforeReverseYIndex = uint32_t((position.y + kBlockHeight / 2) / kBlockHeight);
	indexSet.yIndex = uint32_t(kNumBlockVirtical - 1 - beforeReverseYIndex);
	return indexSet;
}

BlockRect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {
	// 左下中心座標
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex); // 指定ブロックの中心座標を取得する

	BlockRect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;

	return rect;
}
float MapChipField::GetkBlockWidth() { return kBlockWidth; }
