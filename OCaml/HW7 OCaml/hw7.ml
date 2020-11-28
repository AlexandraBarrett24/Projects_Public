(*Ian Colson id:colso031 *)

module type STACK =
sig
  type 'a t
  val empty : 'a t
  val is_empty : 'a t -> bool
  val push : 'a -> 'a t -> 'a t
  val pop : 'a t -> ('a * 'a t) option
end

module type QUEUE =
sig
  type 'a t
  val empty : 'a t
  val is_empty : 'a t -> bool
  val enqueue : 'a -> 'a t -> 'a t
  val dequeue : 'a t -> ('a * 'a t) option
end

(* This is a simple implementation of stacks using `list`. *)
module Stack : STACK =
struct
  type 'a t = 'a list
  let empty = []
  let is_empty = fun l -> match l with [] -> true | _ -> false
  let push = fun x -> fun l -> x :: l
  let pop = fun l -> match l with [] -> None | (x :: l') -> Some (x, l') (*returns a tuple with the element we poped off and the remaining stack*)
end

(* A placeholder for Homework 7. You might want to remove it before the submission
 * to make sure you have filled in every function. *)


module Queue =
struct

  type 'a t = 'a Stack.t * 'a Stack.t

  let enqueue
    = fun x -> fun q ->
      match q with
      | (s1, s2) -> (s1, Stack.push x s2)
(*s1 = front s2 = back*)
  let empty = (Stack.empty, Stack.empty)

  let is_empty : 'a . 'a t -> bool
    = fun q ->
      match q with
      | (s1, s2) ->
        match Stack.is_empty s1 with
        | false -> false
        | true -> Stack.is_empty s2

  (* `dneppa s1 s2 = s1 @ rev s2` *)
  let rec dneppa : 'a . 'a Stack.t -> 'a Stack.t -> 'a Stack.t
    = fun s1 s2 ->
      match Stack.pop s2 with
      | None -> s1
      | Some (x, s2') -> dneppa (Stack.push x s1) s2'

  let reverse : 'a . 'a Stack.t -> 'a Stack.t
    = fun s -> dneppa Stack.empty s

  (* Lab Exercise *)
  let dequeue : 'a . 'a t -> ('a * 'a t) option
    = fun q ->
    match q with
    |(front, back) ->
      match Stack.pop front with
      |Some (x, front') -> Some (x, (front', back))
      |None -> let s1' = reverse back in match Stack.pop s1' with
                                      | None -> None
                                      |Some (x, s1'') -> Some (x, (s1'', Stack.empty))

end

module SealedQueue : QUEUE = Queue

module type EXTENDED_QUEUE =
sig
  include QUEUE

  (* `dequeue_all q` keeps removing elements from the queue `q`
   * until it is empty, and returns a list of removed elements.
   *
   * The first element removed from the `q` is the head of the
   * outputted list. *)
  val dequeue_all : 'a t -> 'a list

  (* `enqueue_all l q` keeps enqueuing elements from the list `l` to `q`,
   * starting from the head of `l`, and returns the resulting queue. *)
  val enqueue_all : 'a list -> 'a t -> 'a t

  (* `append q1 q2` concatenates two queues `q1` and `q2` where
   * the head of the resulting queue is the head of `q1`. *)
  val append : 'a t -> 'a t -> 'a t

  (* `reverse q` flips the direction of `q`. The head of the new queue
   * is the end of the original queue. Hints: you will probably overthink
   * on this---Favonia can implement this in 20 characters with all the short
   * forms. *)
  val reverse : 'a t -> 'a t
end

module ExtendedQueue : EXTENDED_QUEUE =
struct
  include Queue
  let rec dequeue_all : 'a t -> 'a list
    = fun q ->
    match Queue.dequeue q with
    |Some (elem, rest) -> elem :: (dequeue_all rest)
    |None -> []

  let rec enqueue_all : 'a list -> 'a t -> 'a t
    = fun l -> fun q ->
    match l with
    |[] -> q
    |(h :: t) ->  enqueue_all t (Queue.enqueue h q)

    let rec app_help : 'a Stack.t -> 'a list=
    fun s1 ->
    match Stack.pop s1 with
    |None -> []
    |Some (element, rest_of_stack) -> element :: (app_help rest_of_stack)

  let append : 'a t -> 'a t -> 'a t
    = fun q1 -> fun q2->
    match Queue.is_empty q1 with
    |true -> q2
    |false -> match Queue.is_empty q2 with
              |true -> q1
              |false -> match q2 with
                        |(s1, s2) -> enqueue_all (app_help s1) q1 (* the [] in this line needs to be s1*)
        (* `append q1 q2` concatenates two queues `q1` and `q2` where
         * the head of the resulting queue is the head of `q1`. *)



  let reverse : 'a t -> 'a t
    = fun q ->
    match Queue.is_empty q with
    |true -> q
    |_->
            match q with
            |(front, back) ->
                            (match Stack.is_empty front with
                            |true -> (back, front)
                            |_-> let newAns = enqueue_all (app_help front) q in (*in this case [] needs to be front*)
                                match newAns with
                                |(frontt, backk) -> (backk, frontt)) (*in this case, the second use of frontt should be []*)

    (* `reverse q` flips the direction of `q`. The head of the new queue
     * is the end of the original queue. Hints: you will probably overthink
     * on this---Favonia can implement this in 20 characters with all the short
     * forms. *)

end

(*decided not to change function*)

module type SCOPE =
sig
  (* `'a scope` keeps track of bindings, where `'a` is the type of value representations.
   * A binding will be either a value binding or a module binding, and the purpose of
   * this exercise is to implement recursive name resolution. *)
  type 'a scope
  type 'a t = 'a scope

  (** `empty` returns an empty scope *)
  val empty : 'a scope

  (** `lookup_module` takes a list of names and recursively resolves the names
    * to find the module. It returns a scope that has all the bindings exported
    * by the module.
    *
    * For example, `lookup_module ["M"; "N"; "O"]` will locate and return a scope
    * having all the bindings exported by the module `M.N.O`.
    *
    * The function should raise `Not_found` if it cannot find the binding. *)
  val lookup_module : string list -> 'a scope -> 'a scope

  (** `lookup_value` is similar to `lookup_module` except that it returns a value
    * instead.
    *
    * For example, `lookup_value ["M"; "N"; "x"]` will locate and return the value
    * that `M.N.x` is bound to.
    *
    * The function should raise `Not_found` if it cannot find the binding. *)
  val lookup_value : string list -> 'a scope -> 'a

  (** `bind_value (x, v) s` returns a new scope that extends the old scope `s` with
    * a new value binding that binds `x` to `v`.
    *
    * You may assume the name `x` is never capitalized. *)
  val bind_value : string * 'a -> 'a scope -> 'a scope

  (** `bind_module (x, m) t` returns a new scope that extends the old scope `s` with
    * a new module binding that binds `x` to `m`. The scope `m` represents all the bindings
    * exported by the module.
    *
    * You may assume the name `x` is always capitalized. See `bind_value`. *)
  val bind_module : string * 'a scope -> 'a scope -> 'a scope

  (** `append` concatenates two scopes together.
    *
    * In the new scope returned by `append s1 s2`, the bindings in `s2` will shadow
    * the bindings in `s1`. That is, the concatenation is done is the textual order. *)
  val append : 'a scope -> 'a scope -> 'a scope
end

module Scope : SCOPE =
struct
  (** This `'a scope` is just a suggested implementation. Feel free to modify it.
    * No one can tell how you implement it after the sealing! *)
  type 'a scope = (string * 'a item) list
  and 'a item = DVal of 'a | DModule of 'a scope
  type 'a t = 'a scope
  let empty = []

  let rec lm_help : string -> 'a scope -> 'a item =
  fun name -> fun other ->
  match other with
  |[] -> raise Not_found
  |(strin, itm) :: e -> match strin = name with
            |true -> itm
            |_-> lm_help name e

  let rec lookup_module : string list -> 'a scope -> 'a scope =
  fun names -> fun sc ->
  match  names with
  | [] -> sc
  |x :: names' -> match lm_help x sc with
                  |DVal v -> raise Not_found
                  |DModule sc' -> lookup_module names' sc'




  (** `lookup_module` takes a list of names and recursively resolves the names
    * to find the module. It returns a scope that has all the bindings exported
    * by the module.
    *
    * For example, `lookup_module ["M"; "N"; "O"]` will locate and return a scope
    * having all the bindings exported by the module `M.N.O`.
    *
    * The function should raise `Not_found` if it cannot find the binding. *)


  let rec lv_help : string -> 'a scope -> 'a item =
  fun name -> fun other ->
  match other with
  |[] -> raise Not_found
  |(strin, itm) :: e -> match strin = name with
            |true -> itm
            |_-> lv_help name e

  let rec lookup_value : string list -> 'a scope -> 'a =
  fun names -> fun sc ->
  match  names with
  | [] -> raise Not_found
  |x :: names' -> match (lv_help x sc, names = [x]) with
                  |(DVal v, true) -> v
                  |(DVal p, false) -> raise Not_found
                  |(DModule sc', _) -> lookup_value names' sc'
                  (** `lookup_value` is similar to `lookup_module` except that it returns a value
                    * instead.
                    *
                    * For example, `lookup_value ["M"; "N"; "x"]` will locate and return the value
                    * that `M.N.x` is bound to.
                    *
                    * The function should raise `Not_found` if it cannot find the binding. *)

  let bind_value :  string * 'a -> 'a scope -> 'a scope = fun b -> fun sc ->
  match b with
  |(stin, itm) -> (stin, DVal itm) :: sc


  let bind_module : string * 'a scope -> 'a scope -> 'a scope = fun sc1 -> fun sc2 ->
  match sc1 with
  |(stin, itm) -> (stin, DModule itm) :: sc2

  let rec append : 'a scope -> 'a scope -> 'a scope =
  fun s1 -> fun s2 ->
  match s1 with
  |[] -> s2
  |h :: t -> h :: (append t s2)
                      (** `append` concatenates two scopes together.
                        *
                        * In the new scope returned by `append s1 s2`, the bindings in `s2` will shadow
                        * the bindings in `s1`. That is, the concatenation is done is the textual order. *)
end
