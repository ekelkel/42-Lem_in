/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agelloz <agelloz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/14 16:33:56 by agelloz           #+#    #+#             */
/*   Updated: 2019/11/29 19:20:30 by agelloz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# include "libft.h" 

# define INCREASE 1
# define DECREASE -1

typedef struct	s_edge
{
	size_t			dest;
	size_t			capacity;
	struct s_edge	*next;
}				t_edge;

typedef struct	s_node
{
	t_edge	*head;
	int		index;
	int		x_coord;
	int		y_coord;
	int8_t	source;
	int8_t	sink;
	int8_t	bfs_marked;
	int8_t	enqueued;
	int8_t	enqueued_backward;
	char	pad[7];
	char	*name;
}				t_node;

typedef struct	s_graph
{
	t_node	*nodes;
	size_t	ants;
	size_t	size;
	size_t	source;
	size_t	sink;
	size_t	paths_count;
	size_t	old_output_lines;
}				t_graph;

typedef struct	s_parsing
{
	size_t	index;
	size_t	ants;
	ssize_t	source;
	ssize_t	sink;
	t_list	*nodes;
	t_list	*x_coord;
	t_list	*y_coord;
	t_list	*from;
	t_list	*to;
	t_list	*file;
	int8_t	is_prev_command;
	char	pad[7];
}				t_parsing;

typedef struct	s_bfs
{
	t_list	*shortest_path;
	t_edge	*neighbours;
	t_edge	*neighbours2;
	ssize_t	*queue;
	ssize_t	*prev;
	size_t	queue_front;
	size_t	queue_rear;
	size_t	queue_size;
	size_t	queue_capacity;
	size_t	node;
}				t_bfs;

typedef struct	s_paths
{
	t_list		**array;
	size_t		*ants_to_paths;
	size_t		*n;
	size_t		*len;
	size_t		output_lines;
	int8_t		*available;
	t_edge		*neighbours;
	t_edge		*neighbours2;
}				t_paths;

typedef struct	s_options
{
	int8_t		help;
	int8_t		visual;
	int8_t		graph;
	int8_t		paths;
}				t_options;

int8_t			parse_file(t_parsing *p);
int8_t			save_node(t_parsing *p, char *line);
int8_t			save_edge(t_parsing *p, char *line);
int8_t			is_ants(char *line);
int8_t			is_command(char *line);
int8_t			is_node(char *line);
int8_t			is_name_duplicate(t_parsing *p, char *new_name);
int8_t			is_coord_duplicate(t_parsing *p, int x, int y);
int8_t			is_edge_duplicate(t_parsing *p, t_list *new_from,
									t_list *new_to);
int8_t			is_edge(char *line);
int8_t			is_comment_or_false_command(char *line);
int8_t			check_int(char *nb, int8_t pos);

t_graph			*build_graph(t_parsing *p);
int8_t			create_edge(t_graph *graph, size_t src, size_t dest);

t_list			*find_paths(t_graph *graph);
t_bfs			*init_bfs(t_graph *graph);
t_list			*rebuild_paths(t_graph *graph);
t_paths			*find_solution(t_graph *graph, t_list *aug_paths);
int8_t			is_new_solution_better(t_list *aug_paths, t_graph *graph);
void			update_edge_capacities(t_bfs *new_bfs, t_graph *graph,
										int8_t bfs_succeed);
void			change_capacity(t_graph *graph, t_list *u, t_list *v,
								int8_t order);
int8_t			enqueue(size_t node, size_t neighbour, t_graph *graph,
						t_bfs *bfs);
size_t			dequeue(t_bfs *bfs);
void			reset_marks_fail(t_graph *graph, t_bfs *bfs);
void			reset_marks(t_graph *graph, t_bfs *bfs);

void			print_file(t_parsing *p);

int8_t			solver(t_graph *graph, t_list *aug_paths, t_options *o);
void			assign_ants_to_paths(t_graph *graph, t_paths *paths,
									size_t tmp[]);
t_paths			*init_output(t_graph *graph, t_list *aug_paths);
void			init_lines(t_paths *paths, t_graph *graph);
int8_t			is_solution_found(t_paths *paths, t_graph *graph);
int8_t			is_new_solution_better(t_list *aug_paths, t_graph *graph);
t_list			*get_next_path(t_list *path, t_graph *graph);
size_t			compute_path_pos(t_list **path, t_list *aug_paths,
									t_graph *graph);
size_t			is_on_path(size_t node, t_list *path, t_graph *graph);
t_bfs			*reconstruct_path(t_bfs *new_bfs, t_graph *graph);
void			skip_node(t_bfs *new_bfs, t_edge *neigh, t_graph *graph,
							t_list *path);
int8_t			is_source_neighbours(size_t node, t_graph *graph);
int8_t			reset_availability(t_graph *graph, t_paths *paths,
									size_t *ants2paths);
int8_t			all_moved(t_list **pos, ssize_t *tmp, t_graph *graph,
							t_paths *paths);
void			print_lines(t_paths *paths, t_graph *graph);

void			print_ssize_t(t_list *list, t_graph *graph);
void			print_graph(t_graph *graph);

void			free_p(t_parsing *p);
void			free_tab(char **tab);
void			free_graph(t_graph *graph);
t_bfs			*free_bfs(t_bfs *bfs);
t_paths			*free_paths(t_paths *paths, t_graph *graph);
int8_t			exit_parsing_error(t_parsing *p, char *line,
									char **tab);
int				exit_bfs_error(t_parsing *p, t_graph *graph);

int8_t			visualizer(t_graph *graph, t_paths *paths);

#endif
