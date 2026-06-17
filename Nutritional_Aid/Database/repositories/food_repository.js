import pool from "../connections.js";

export async function bulk_insert_foods(foodBatch) {
    const values = foodBatch.map(food => [
        food.display_name,
        food.calories,
        food.carbs,
        food.protein,
        food.fat,
        food.servingSize,
        food.nutriScore
    ]);

    const sql = `
        INSERT INTO foods
        (
            name,
            calories,
            carbs,
            protein,
            fat,
            servingSize,
            nutriScore,
        )
        VALUES ?
    `;

    return pool.query(sql, [values]);
}

export async function getFoodById(id) {
    const sql = `
        SELECT * FROM foods
        WHERE id = ?
    `;

    const [rows] = await pool.query(sql, [id]);
    return rows[0];
}

export async function get_foods(){
    const sql = `
        SELECT * FROM foods
        LIMIT 10
    `;

    const [rows] = await pool.query(sql);
    return rows;
}

export async function create_food(food) {
    const sql = `
        INSERT INTO foods
        (
            name,
            calories,
            carbs,
            protein,
            fat,
            servingSize,
            nutriScore,
        )
        VALUES (?, ?, ?, ?, ?, ?, ?)
    `;

    const [result] = await pool.query(sql, [
        food.name,
        food.calories,
        food.carbs,
        food.protein,
        food.fat,
        food.servingSize,
        food.nutriScore
    ]);

    return result.insertId;
}

export async function update_food(id, food) {
    const sql = `
        UPDATE foods
        SET name = ?, calories = ?, carbs = ?, protein = ?, fat = ?, servingSize = ?, nutriScore = ?
        WHERE id = ?
    `;

    await pool.query(sql, [
        food.name,
        food.calories,
        food.carbs,
        food.protein,
        food.fat,
        food.servingSize,
        food.nutriScore,
        id
    ]);
}

export async function delete_food(id) {
    const sql = `
        DELETE FROM foods
        WHERE id = ?
    `;

    await pool.query(sql, [id]);
}