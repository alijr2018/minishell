/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sellyani <sellyani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:06:21 by sellyani          #+#    #+#             */
/*   Updated: 2024/11/09 15:06:04 by sellyani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *head, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*old;

	if (!head || !f)
		return (NULL);
	new = NULL;
	while (head)
	{
		old = ft_lstnew (f(head->content));
		if (!old)
		{
			ft_lstclear(&old, (*del));
			return (NULL);
		}
		ft_lstadd_back(&new, old);
		head = head->next;
	}
	return (new);
}

void *ft_add(void *ad){
	char *s = malloc (ft_strlen(ad) + 3); // 78
	if (!s)
		return 0;
	sprintf(s,"%s78\n",ad);
	return s;
}
void *add_plus(void *c){
	c +=1;
}

int main(){
	int i = 0;
	t_list *n = NULL;
	
	while(i <= 100){
		ft_lstadd_back(&n ,ft_lstnew((void *)i));
		i++;
	}
	
	
	// while(n){t;
	// 	printf("%d\n",n->content);
	// 	n = n->nex
	// }
	printf("*************------********");

	t_list *r =ft_lstmap(n,ft_add,free);
	t_list *t =r;
	while(t){
		printf("%s\n",t->content);
		t = t->next;
	}
}