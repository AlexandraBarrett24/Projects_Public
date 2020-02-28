open Hw5

exception NotNum of value (* The evaluator expects a number, but it is not. *)
exception NotFun of value (* The evaluator expects a function, but it is not. *)
exception UnsupportedRecursiveDefinition of expr (* The evaluator does not support this recursive definition. *)
exception UnboundVarriable of string (* The variable is unbound! *)
exception NoMatch (* We fail to find a pattern that matches the value. *)

let lookup : 'a . string -> (string * 'a) list -> 'a
  = fun x -> fun env ->
    try List.assoc x env
    with
    | Not_found -> raise (UnboundVarriable x)

let value_of_bool : bool -> value
  = fun b ->
    match b with
    | true -> VTrue
    | false -> VFalse

(* Exercise 4: Finish the Mini ML interpreter *)

(* `eval` is the evaluation function which takes a list of bindings and an
 * expression, and returns the result. *)
let rec eval : (string * value) list -> expr -> value
  = fun env -> fun expr ->
    match expr with
    | EVar x -> lookup x env
    | ELet ((x, e1), e2) ->
      let v1 = eval env e1 in
      let env' = (x,v1) :: env in
      eval env' e2
    | ELetRec ((f, EFun (x, e1)), e2) ->
      let env' = (f,VRecFun (env, f, x, e1)) :: env in
      eval env' e2
    | ELetRec ((_, e1), _) ->
      raise (UnsupportedRecursiveDefinition e1)
    | ENum i -> VNum i
    | EMinus e ->
    let i1 = eval_num env e in
    VNum (il * -1)
  (* `EMinus e` represents `- e` *)
    | EAdd (e1, e2) ->
      let i1 = eval_num env e1 in
      let i2 = eval_num env e2 in
      VNum (i1 + i2)
    | ESub (e1, e2) ->
    let i1 = eval_num env e1 in
    let i2 = eval_num env e2 in
    VNum (i1 - i2)
      (* `ESub (e1, e2)` represents `e1 - e2` *)
    | EMult (e1, e2) ->
    let i1 = eval_num env e1 in
    let i2 = eval_num env e2 in
    VNum (i1 * i2)
    (* `EMult (e1, e2)` represents `e1 * e2` *)
    | EEqual (e1, e2) ->
      let i1 = eval_num env e1 in
      let i2 = eval_num env e2 in
      value_of_bool (i1 = i2)
    | ELess (e1, e2) ->
    let i1 = eval_num env e1 in
    let i2 = eval_num env e2 in
    (match (i1 < i2) with
    |true -> VTrue
    |false ->VFalse)
    (* `ELess (e1, e2)` represents `e1 < e2` *)
    | EGreater (e1, e2) ->
    let i1 = eval_num env e1 in
    let i2 = eval_num env e2 in
    (match (i1 > i2) with
    |true -> VTrue
    |false ->VFalse)
    (* `EGreater (e1, e2)` represents `e1 > e2` *)
    | ETrue -> VTrue (*me*)
    | EFalse -> VFalse
    (* `EFalse` represents `false` *)
    | EMatch (e, l) ->
      let v = eval env e in
      eval_match env l v
    | ETuple l ->
    (match l with
    |[] -> VTuple []
    |h :: t ->  let m = eval env h in
                let rest = (eval env (ETuple t))
                VTuple (m  :: rest :: []) 

    )
    (* `ETuple []` represents `()` *)
    (* `ETuple [e1; e2; ...; en]` (make a new list with the values that have been evaluated to make srue they are right) represents `(e1, e2, ..., en)` *)

    | ENone ->
    VNone
        (* `ENone` represents `None` *)
    | ESome e ->
    Some e
    (* `ESome e` represents `Some e` *)
    | EFun (x, e) -> VFun (env, x, e)
    | EApply (e1, e2) ->
      let v1 = eval env e1 in
      let v2 = eval env e2 in
      (match v1 with
       | VFun (env', x, e1') ->
         eval ((x, v2)::env') e1'
       | VRecFun (env', f, x, e1') ->
         eval ((x, v2)::(f, v1)::env') e1'
       | v -> raise (NotFun v))
    | ENil -> []
    (* `[]` *)
    | ECons (e1, e2) ->
    let i1 = eval env e1 in
    let i2 = eval env e2 in
    VCons (i1, i2)
    (* `e1 :: e2` *)

(* `let rec ... and ... and ...` creates mutual recursion *)
and eval_num : (string * value) list -> expr -> int
  = fun env -> fun e ->
    match eval env e with
    | VNum i -> i
    | v -> raise (NotNum v)

and eval_match : (string * value) list -> (pattern * expr) list -> value -> value
  = fun env -> fun l ->  fun val ->
  match l with
  |[] -> raise NoMatch
  |(pat, ex) :: t -> match match_value pat val with
                    | None -> eval_match env t val
                    | Some e -> eval env ex
