import pool from '../connections.js';
// technically bad design decision as this file should only have sql
// should move processing of token batch to script file but for now this is fine

// this solution is also rather bad because it relies on the fact that the ids of the inserted 
// foods are sequential and start from the firstid, which may not always be the case if there 
// are other inserts happening concurrently. A better solution would be to return the inserted 
// ids from the bulk insert and use those to insert the tokens, but that would require changing 
// the bulk_insert_foods function to return the inserted ids.

// the post request for creating foods also doesnt create tokens, which is a problem, 
// but for now we can just focus on the bulk insert from the script file and worry about the post 
// request later.


export async function insert_food_tokens(firstid, batch){
    // Implementation for bulk inserting food tokens  
    const tokenbatch = []; 
    for (let i = 0; i < batch.length; i++) {
        const food = batch[i];
        const foodId = firstid + i; // Assuming the IDs are sequential and start from firstid

        const tokens = food.tokens; // Assuming tokens is an array of strings

        for (const token of tokens) {
            tokenbatch.push([foodId, token]);
        }
    }
    // Execute bulk insert
    await pool.query(
        'INSERT INTO food_tokens (food_id, token) VALUES ?',
        [tokenbatch]
    );
}