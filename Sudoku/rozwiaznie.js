function conv(arr) {
    let out = [];
    for(let i = 0; i < 9; i++) out.push([]);
    for(let i = 0; i < arr.length; i++) {
        out[Math.floor(i / 9)][i % 9] = arr[i];
    }
    return out;
}
        //Check if table is legal; accepts flat array
function test(arrs) {
    let arr = conv(arrs)
    function grids(arr) {
        let out = [];
        for(let i = 0; i < 3; i++)
        for(let j = 0; j < 3; j++) {
            let g = [];
            for(let x = 0; x < 3; x++)
            for(let y = 0; y < 3; y++) 
                if(arr.length > (i * 3) + x)
                g.push(arr[(i * 3) + x][(j * 3) + y]);
            out.push(g)
        }
        return out;
    }
    arrtrans = arr[0].map((_, colIndex) => arr.map(row=>row[colIndex]));
    let g = grids(arr);
    for(i of [arr, arrtrans, g]) {
        if(i.some(hasDuplicates))
            return false;
    }
    return true;
}
        //Sorting random index
function random_sort(arr) {
    for(let i = 0; i < arr.length; i++) {
        let index = Math.floor(Math.random() * (arr.length - i)) + i;
        let tmp = arr[index];
        arr[index] = arr[i];
        arr[i] = tmp;
    }
}
        //Generating numbers 1-9 to add to cells
function generate(arr) {
    if(arr.length == 81) return true;
    let nums = [1, 2, 3, 4, 5, 6, 7, 8, 9]
    random_sort(nums);
    for(i of nums) {
        arr.push(i);
        if(test(arr))
            if (generate(arr)) return true;
        arr.pop(i);
    }
    return false;
}