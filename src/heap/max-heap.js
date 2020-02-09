/**
 * 数组元素交换
 * @param {*} A 
 * @param {*} i 
 * @param {*} j 
 */
function swap(A, i, j) {
  const t = A[i]
  A[i] = A[j]
  A[j] = t
}

/**
 * 最大堆
 */
class MaxHeap {

  constructor(data) {
      this.list = [...data]
      for (let i = 0; i < data.length; i++) {
          this.list[i] = data[i]
      }
      this.heapSize = data.length
      this.build()
  }


  //建堆
    build() {
      let i = Math.floor(this.heapSize / 2) - 1
      while (i >= 0) {
          // 自底向上调整, 每个节点一个循环
          this.max_heapify(i--)
      }
  }

  //最大[堆化]
  max_heapify(i) {
      const leftIndex = 2 * i + 1
      const rightIndex = 2 * i + 2
      let maxIndex = i
      if (leftIndex < this.heapSize && this.list[leftIndex] > this.list[maxIndex]) {
          maxIndex = leftIndex
      }
      if (rightIndex < this.heapSize && this.list[rightIndex] > this.list[maxIndex]) {
          maxIndex = rightIndex
      }
      if (i !== maxIndex) {
          swap(this.list, maxIndex, i)
          // 自顶向下调整
          this.max_heapify(maxIndex)
      }
  }
  
  //提取最大堆第一个节点并恢复堆为最大堆
  extract() {
      if (this.heapSize === 0) return null
      const item = this.list[0]
      swap(this.list, 0, this.heapSize - 1)
      this.heapSize--
      this.max_heapify(0)
      return item
  }

  //增加一个元素
  insert(item) {
      this.list.push(item);
      this.heapSize++
      this.build();
  }

  print() {
    return JSON.stringify(this.list)
  }

  size() {
      return this.list.length;
  }
    
  // 堆排序
  sort() {
    const result = []
    let i = this.heapSize
    while ( i > 0) {
        result.push(heap.extract())
        i--
    }
      
    return result
  }
}

const array = [12, 15, 2, 4, 3, 8, 7, 6, 5]

const heap = new MaxHeap(array)
console.log('最大二叉堆是：', heap.print())

heap.insert(9)

console.log('插入9之后的最大二叉堆是：', heap.print())

heap.extract()

console.log('删除根节点之后的最大二叉堆是：', heap.print())

console.log('二叉堆进行堆排序结果：', heap.sort())

console.log(heap.print())