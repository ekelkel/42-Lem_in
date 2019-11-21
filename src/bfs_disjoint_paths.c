/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs_disjoint_paths.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelkel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 16:12:57 by ekelkel           #+#    #+#             */
/*   Updated: 2019/11/21 21:04:58 by agelloz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

t_list			*rebuild_paths(t_graph *graph)
{
	t_list		*new_aug_paths;
	t_list		*tmp;
	t_edge		*neighbours;
	t_edge		*neighbours2;
	size_t		i;
	size_t		node;

	new_aug_paths = NULL;
	tmp = NULL;
	i = 0;
	node = graph->source;
	neighbours = graph->nodes[graph->source].head;
	while (neighbours != NULL)
	{
		if (neighbours->capacity == 0)
		{
			tmp = ft_lstnew(&graph->source, sizeof(size_t));
			ft_lstappend(&new_aug_paths, tmp);
			node = neighbours->dest;
			tmp = ft_lstnew(&node, sizeof(size_t));
			ft_lstappend(&new_aug_paths, tmp);
			neighbours2 = graph->nodes[node].head;
			while (node != graph->sink)
			{
				if (neighbours2->capacity == 0)
				{
					node = neighbours2->dest;
					tmp = ft_lstnew(&node, sizeof(size_t));
					ft_lstappend(&new_aug_paths, tmp);
					neighbours2 = graph->nodes[node].head;
				}
				else
					neighbours2 = neighbours2->next;
			}
		}
		neighbours = neighbours->next;
	}
	return (new_aug_paths);
}

size_t			is_on_path(size_t node, t_list *aug_paths, t_graph *graph)
{
	t_list	*curr;
	size_t	path_nb;

	path_nb = 0;
	curr = aug_paths;
	while (curr != NULL)
	{
		if (*(size_t *)curr->content == node && node != graph->source)
			return (path_nb);
		curr = curr->next;
	}
	return (FALSE);
}

int8_t			reconstruct_path2(t_edge *from, t_bfs *bfs, t_graph *graph)
{
	t_list	*tmp;
	ssize_t	i;

	tmp = NULL;
	i = graph->size - 1;
	while (graph->nodes[i].sink == FALSE)
		i--;
	while (i != -1)
	{
		tmp = ft_lstnew(&i, sizeof(ssize_t));
		ft_lstadd(&bfs->shortest_path, tmp);
		i = bfs->prev[i];
	}
	if (*(size_t *)bfs->shortest_path->content != from->dest)
	{
		reset_marks_fail(graph, bfs, FALSE);
		ft_lstdel(&bfs->shortest_path, ft_delcontent);
		free_bfs(bfs);
		return (FALSE);
	}
	reset_marks(graph, bfs, FALSE);
	free_bfs(bfs);
	return (TRUE);
}

int8_t			path_found(t_edge *from, t_graph *graph)
{
	size_t	node;
	t_bfs	*bfs;
	t_edge	*neighbours;

	neighbours = NULL;
	bfs = init_bfs(graph, FALSE);
	bfs->queue[0] = from->dest;
	bfs->queue_size = 1;
	graph->nodes[from->dest].tmp_marked = TRUE;
	while (is_queue_empty(bfs) == FALSE)
	{
		node = dequeue(bfs);
		neighbours = graph->nodes[node].head;
		while (neighbours)
		{
			if (graph->nodes[neighbours->dest].tmp_marked != TRUE
					&& neighbours->capacity > 0)
			{
				enqueue(bfs, neighbours->dest);
				bfs->prev[neighbours->dest] = node;
				graph->nodes[neighbours->dest].tmp_marked = TRUE;
			}
			neighbours = neighbours->next;
		}
	}
	return (reconstruct_path2(from, bfs, graph));
}


size_t			can_we_escape_from(size_t node, t_graph *graph)
{
	t_edge		*curr;

	curr = graph->nodes[node].head;
	while (curr != NULL)
	{
		ft_printf("esc:%d neigh:%d, mark:%d\n", node, curr->dest, graph->nodes[curr->dest].bfs_marked);
		if (graph->nodes[curr->dest].bfs_marked == FALSE)
		{
			if (path_found(curr, graph) == TRUE)
				return (TRUE);
			return (FALSE);
		}
		curr = curr->next;
	}
	return (FALSE);
}


t_bfs			*bfs_disjoint_paths(t_graph *graph, t_list *aug_paths)
{
	size_t	node;
	t_bfs	*bfs;
	t_edge	*neighbours;

	neighbours = NULL;
	bfs = init_bfs(graph, TRUE);
	while (is_queue_empty(bfs) == FALSE)
	{
		node = dequeue(bfs);
		neighbours = graph->nodes[node].head;
		ft_printf(">> dq:%d\n", node);
		if (is_on_path(node, aug_paths, graph) == TRUE)
		{
			while (neighbours)
			{
				if (can_we_escape_from(neighbours->dest, graph) == TRUE)
				{
					enqueue(bfs, neighbours->dest);
					ft_printf("ESC node:%d (%s) q:%d\n", node, graph->nodes[node].name, neighbours->dest);
					bfs->prev[neighbours->dest] = node;
					graph->nodes[neighbours->dest].bfs_marked = TRUE;
					graph->nodes[neighbours->dest].already_enqueued = TRUE;
					break ;
				}
				else if (graph->nodes[neighbours->dest].already_enqueued == FALSE && neighbours->capacity == 2)
				{
					enqueue(bfs, neighbours->dest);
					ft_printf("ON PATH node:%d (%s) q:%d\n", node, graph->nodes[node].name, neighbours->dest);
					bfs->prev[neighbours->dest] = node;
					graph->nodes[neighbours->dest].bfs_marked = TRUE;
					graph->nodes[neighbours->dest].already_enqueued = TRUE;
					break ;
				}
				neighbours = neighbours->next;
			}
		}
		else
		{
			while (neighbours)
			{
				if (graph->nodes[neighbours->dest].already_enqueued == FALSE && graph->nodes[neighbours->dest].bfs_marked == FALSE)
				{
					enqueue(bfs, neighbours->dest);
					ft_printf("node:%d (%s) q:%d\n", node, graph->nodes[node].name, neighbours->dest);
					bfs->prev[neighbours->dest] = node;
					graph->nodes[neighbours->dest].bfs_marked = TRUE;
					graph->nodes[neighbours->dest].already_enqueued = TRUE;
				}
				neighbours = neighbours->next;
			}
		}
	}
	return (reconstruct_path(bfs, graph));
}

t_list	*find_disjoint_paths(t_graph *graph, t_list *aug_paths)
{
	t_bfs	*new_bfs;
	t_list	*curr_path_node;

	while (TRUE)
	{
		if ((new_bfs = bfs_disjoint_paths(graph, aug_paths)) == NULL)
			return (aug_paths);
		curr_path_node = new_bfs->shortest_path;
		while (curr_path_node->next != NULL)
		{
			change_capacity(graph, curr_path_node, curr_path_node->next, DECREASE);
			change_capacity(graph, curr_path_node->next, curr_path_node, INCREASE);
			curr_path_node = curr_path_node->next;
		}
		ft_lstdel(&aug_paths, ft_delcontent);
		aug_paths = rebuild_paths(graph);
		free_bfs(new_bfs);
	}
	return (aug_paths);
}
