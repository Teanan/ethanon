/*--------------------------------------------------------------------------------------
 Ethanon Engine (C) Copyright 2008-2013 Andre Santee
 http://ethanonengine.com/

	Permission is hereby granted, free of charge, to any person obtaining a copy of this
	software and associated documentation files (the "Software"), to deal in the
	Software without restriction, including without limitation the rights to use, copy,
	modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
	and to permit persons to whom the Software is furnished to do so, subject to the
	following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
	CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
	OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
--------------------------------------------------------------------------------------*/

#ifndef ETH_BUCKET_MANAGER_H_
#define ETH_BUCKET_MANAGER_H_

class ETHRenderEntity;
class ETHSpriteEntity;
class ETHEntity;
class ETHEntityArray;

#include "ETHSceneProperties.h"
#include "ETHEntityKillListener.h"
#include "../Resource/ETHResourceProvider.h"
#include <list>

// Vector2 hash function
namespace boost {
std::size_t hash_value(Vector2 const& p);
} // namepace boost

#include <boost/unordered/unordered_map.hpp>

typedef std::list<ETHRenderEntity*> ETHEntityList;
typedef boost::unordered_map<Vector2, ETHEntityList, boost::hash<Vector2> > ETHBucketMap;

class ETHEntityChooser;

class ETHBucketManager
{
public:
	static Vector2 GetBucket(const Vector2 &v2, const Vector2 &v2BucketSize);
	static void GetIntersectingBuckets(std::list<Vector2> &outList, const Vector2 &v2Pos, const Vector2 &v2Size,
		const Vector2 &v2BucketSize, const bool includeUpperSeams = false, const bool includeLowerSeams = false);

	ETHBucketManager(const ETHResourceProviderPtr& provider, const Vector2& bucketSize, const bool drawingBorderBuckets);
	~ETHBucketManager();

	enum SIDE
	{
		FRONT = 0,
		BACK = 1
	};

	ETHBucketMap::iterator GetFirstBucket();
	ETHBucketMap::iterator GetLastBucket();
	ETHBucketMap::iterator Find(const Vector2& key);
	ETHBucketMap::const_iterator Find(const Vector2& key) const;
	ETHBucketMap::const_iterator GetFirstBucket() const;
	ETHBucketMap::const_iterator GetLastBucket() const;

	bool IsEmpty() const;
	bool IsDrawingBorderBuckets() const;
	std::size_t GetNumEntities(const Vector2& key) const;
	void SetBorderBucketsDrawing(const bool enable);

	void Add(ETHRenderEntity* entity, const SIDE side);
	const Vector2& GetBucketSize() const;

	unsigned int GetNumEntities() const;

	/// Get the list of visible buckets
	void GetIntersectingBuckets(std::list<Vector2>& bucketList, const Vector2& pos, const Vector2& size, const bool upperSeams, const bool lowerSeams);

	/// Search for an entity whose location collides with the 'at' point
	/// It returns the entity ID # and it's further data. If nAfterThisID is greater than
	/// -1 the entity returned will be another one (if the point collides with more than
	/// one entities)
	int SeekEntity(const Vector2& pointAbsPos, ETHEntity** pOutData, const ETHSceneProperties& props, ETHSpriteEntity* pAfterThisOne = 0);

	/// Seek the entity by ID #
	ETHSpriteEntity *SeekEntity(const int id);

	/// Seek the entity by it's original file name file name
	ETHSpriteEntity *SeekEntity(const str_type::string& fileName);

	/// Delete the entity by ID #
	bool DeleteEntity(const int id, const Vector2 &searchBucket, const bool stopSfx = true);

	/// Delete the entity by ID #
	bool DeleteEntity(const int id);

	/// get an array of pointers with all entities named 'name' in scene
	void GetEntityArrayByName(const str_type::string& name, ETHEntityArray &outVector);

	/// get an array of pointers with all entities in the bucket
	void GetEntityArrayFromBucket(const Vector2 &bucket, ETHEntityArray &outVector);
	void GetEntityArrayFromBucket(const Vector2 &bucket, ETHEntityArray &outVector, const ETHEntityChooser& chooser);
	void GetWhiteListedEntityArrayFromBucket(const Vector2 &bucket, ETHEntityArray &outVector, const str_type::string& semicolonSeparatedNames);

	/// entities around the bucket
	void GetEntitiesAroundBucket(const Vector2& bucket, ETHEntityArray &outVector);
	void GetEntitiesAroundBucket(const Vector2& bucket, ETHEntityArray &outVector, const ETHEntityChooser& chooser);
	void GetWhiteListedEntitiesAroundBucket(const Vector2& bucket, ETHEntityArray &outVector, const str_type::string& semicolonSeparatedNames);
	void GetEntitiesAroundBucketWithBlackList(const Vector2& bucket, ETHEntityArray &outVector, const str_type::string& semicolonSeparatedNames);

	/// get an array of visible entities
	void GetVisibleEntities(ETHEntityArray &outVector);

	/// get an array containing all entities that intersect with the point
	void GetIntersectingEntities(const Vector2 &point, ETHEntityArray &outVector, const bool screenSpace, const ETHSceneProperties& props);

	/// get an array containing all entities in scene
	void GetEntityArray(ETHEntityArray &outVector);

	void RequestBucketMove(ETHEntity* target, const Vector2& oldPos, const Vector2& newPos);

	void ResolveMoveRequests();

	void SetDestructionListener(const ETHEntityKillListenerPtr& listener);

private:

	class ETHBucketMoveRequest
	{
		Vector2 oldBucket, newBucket;
		ETHEntity* entity;
	public:
		ETHBucketMoveRequest(ETHEntity* target, const Vector2& oldPos, const Vector2& newPos, const Vector2& bucketSize);
		bool IsABucketMove() const;
		int GetID() const;
		const Vector2& GetOldBucket() const;
		const Vector2& GetNewBucket() const;
		bool IsAlive() const;
	};

	bool MoveEntity(const int id, const Vector2 &currentBucket, const Vector2 &destBucket);

	std::list<ETHBucketMoveRequest> m_moveRequests;

	ETHResourceProviderPtr m_provider;
	ETHBucketManager& operator=(const ETHBucketManager& p);
	ETHBucketMap m_entities;
	const Vector2 m_bucketSize;
	bool m_drawingBorderBuckets;
	ETHEntityKillListenerPtr m_entityKillListener;
};

#endif
