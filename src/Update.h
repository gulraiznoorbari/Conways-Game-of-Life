#pragma once

#include "UnInitDynamicArray.h"

void Update(bool **grid, UnInitDynamicArray<Position>& cells, int row, int column)
{
	// auxiliary array to keep track of the unique dead cells adjacent to at least one live cell
	UnInitDynamicArray<Position> aux_array(cells.GetSize() * 8);

	int offset[][2] = { {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1} };

	for (int i = 0; i < cells.GetSize(); i++)
	{
		int x, y;
		x = cells[i].x;
		y = cells[i].y;
		cells[i].liveNeighboursCount = 0;
		for (int j = 0; j < 8; j++)
		{
			int temp_x = x + offset[j][0];
			int temp_y = y + offset[j][1];
			if ((temp_x > 0 && temp_x < row) && (temp_y > 0 && temp_y < column))
			{
				if (grid[temp_x][temp_y] == 0)
				{
					Position temp(temp_x, temp_y);
					aux_array.Insert(temp);
					grid[temp_x][temp_y] = 1; // to avoid duplicate entries in the aux array
				}
			}
		}
	}

	for (int i = 0; i < aux_array.GetSize(); i++)
	{
		grid[aux_array[i].x][aux_array[i].y] = 0;
	}

	// live neighbours for dead cells
	for (int i = 0; i < aux_array.GetSize(); i++)
	{
		int x = aux_array[i].x;
		int y = aux_array[i].y;
		for (int j = 0; j < 8; j++)
		{
			int temp_x = x + offset[j][0];
			int temp_y = y + offset[j][1];
			if ((temp_x >= 0 && temp_x < row) && (temp_y >= 0 && temp_y < column))
			{
				if (grid[temp_x][temp_y] == 1)
				{
					aux_array[i].liveNeighboursCount++;
				}
			}
		}
	}

	// live neighbours for alive cells
	for (int i = 0; i < cells.GetSize(); i++)
	{
		int x = cells[i].x;
		int y = cells[i].y;
		for (int j = 0; j < 8; j++)
		{
			int temp_x = x + offset[j][0];
			int temp_y = y + offset[j][1];
			if ((temp_x >= 0 && temp_x < row) && (temp_y >= 0 && temp_y < column))
			{
				if (grid[temp_x][temp_y] == 1)
				{
					cells[i].liveNeighboursCount++;
				}
			}
		}
	}

	// cell dies
	for (int i = 0; i < cells.GetSize(); i++)
	{
		if (cells[i].liveNeighboursCount <= 1 || cells[i].liveNeighboursCount > 3)
		{
			grid[cells[i].x][cells[i].y] = 0;
			Position temp(cells[i].x, cells[i].y);
			cells.Delete(temp);
			i--;
		}
	}

	// cell birth
	for (int i = 0; i < aux_array.GetSize(); i++)
	{
		if (aux_array[i].liveNeighboursCount == 3)
		{
			grid[aux_array[i].x][aux_array[i].y] = 1;
			Position temp(aux_array[i].x, aux_array[i].y);
			cells.Insert(temp);
		}
	}
}