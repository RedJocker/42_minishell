#!/usr/bin/env bats

load ../setup_core

@test "pwd: pwd" {
    assert_minishell_equal_bash "pwd
printf 'Status code: $?\n'"
}

# @test "test export: export hello" {
# @test "test export: export HELLO=123" {
# @test "test export: export A-" {
# @test "test export: export HELLO=123 A" {
# @test "test export: export HELLO=\"123 A-\"" {
# @test "test export: export hello world" {
# @test "test export: export HELLO-=123" {
# @test "test export: export =" {
# @test "test export: export 123" {
